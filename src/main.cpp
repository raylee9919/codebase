// Copyright (c) 2025 Seong Woo Lee. All rights reserved.

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#define UNICODE
#define _UNICODE
#include <windows.h>
#include <dwmapi.h>

#include <d3d11_1.h>
#include <d3dcompiler.h>

// Note: [.h]
#include "Include/Codebase.h"

// Note: [.cpp]
#include "Include/Codebase.cpp"


// Note: Global Variables.
global B32 global_running     = true;
global B32 global_show_cursor = true;


function LRESULT CALLBACK
win32_callback(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    LRESULT result = {};

    switch(msg) 
    {
        case WM_DESTROY: {
            global_running = false;
            PostQuitMessage(0);
        } break;

        case WM_SETCURSOR: {
            if (global_show_cursor) 
            { result = DefWindowProcW(hwnd, msg, wparam, lparam); }
            else 
            { SetCursor(0); }
        } break;

        default: {
            result = DefWindowProcW(hwnd, msg, wparam, lparam);
        } break;
    }

    return result;
}

function
OS_FUNCTION_SHOW_MESSAGE(win32_show_message)
{
#if 0
    LPCWSTR caption = L"This is caption";
    UINT message_box_type = MB_ICONSTOP;
    MessageBoxExW(NULL, msg, caption, message_box_type, 0);
#endif
}

//
// Note: System Info.
//
function
OS_FUNCTION_GET_PAGE_SIZE(win32_get_page_size)
{
    SYSTEM_INFO info;
    GetSystemInfo(&info);
    return info.dwPageSize;
}

function
OS_FUNCTION_GET_LOGICAL_PROCESSOR_COUNT(win32_get_logical_processor_count)
{
    SYSTEM_INFO info;
    GetSystemInfo(&info);
    U32 result = info.dwNumberOfProcessors;
    return result;
}

// Note: Abort
function
OS_FUNCTION_ABORT(win32_abort)
{
    ExitProcess(1);
}

//
// Note: Memory
//
function
OS_FUNCTION_RESERVE(win32_reserve)
{
    DWORD alloc_flags   = MEM_RESERVE;
    DWORD protect_flags = PAGE_NOACCESS;
    if (commit)
    {
        alloc_flags   |= MEM_COMMIT;
        protect_flags = PAGE_READWRITE;
    }

    void *result = VirtualAlloc(0, size, alloc_flags, protect_flags);
    return result;
}

function
OS_FUNCTION_RELEASE(win32_release)
{
    VirtualFree(ptr, 0, MEM_RELEASE);
}

function
OS_FUNCTION_COMMIT(win32_commit)
{
    U64 page_snapped_size = size;
    page_snapped_size += win32_get_page_size() - 1;
    page_snapped_size -= page_snapped_size % win32_get_page_size();
    VirtualAlloc(ptr, page_snapped_size, MEM_COMMIT, PAGE_READWRITE);
}

function
OS_FUNCTION_DECOMMIT(win32_decommit)
{
    VirtualFree(ptr, size, MEM_DECOMMIT);
}

//
// Note: File System
//
function
OS_FUNCTION_OPEN_FILE(win32_open_file)
{
    Temporary_Arena scratch = scratch_begin();

    String16 path16 = string16_from_string8(scratch.arena, path);

    DWORD desired_access = 0;
    if (flags & OS_FILE_ACCESS_READ)  
    { desired_access |= GENERIC_READ; }

    if (flags & OS_FILE_ACCESS_WRITE) 
    { desired_access |= GENERIC_WRITE; }

    DWORD share_mode = 0;
    if (flags & OS_FILE_ACCESS_SHARED) 
    { share_mode = FILE_SHARE_READ; }

    SECURITY_ATTRIBUTES security_attr = {};
    {
        security_attr.nLength              = sizeof(SECURITY_ATTRIBUTES);
        security_attr.lpSecurityDescriptor = NULL;
        security_attr.bInheritHandle       = FALSE;
    };

    DWORD creation_disposition = 0;
    if (flags & OS_FILE_ACCESS_CREATE_NEW)
    { creation_disposition = CREATE_ALWAYS; }
    else
    { creation_disposition = OPEN_EXISTING; }

    DWORD flags_and_attributes = 0;
    HANDLE template_file = NULL;

    HANDLE file = CreateFileW((LPCWSTR)path16.str, desired_access, share_mode, &security_attr,
                              creation_disposition, flags_and_attributes, template_file);

    if (file == INVALID_HANDLE_VALUE)
    { assert(0); }

    Os_Handle handle = {};
    handle.u64 = (U64)file;

    scratch_end(scratch);
    return handle;
}

function
OS_FUNCTION_CLOSE_FILE(win32_close_file)
{
    HANDLE handle = (HANDLE)file.u64;
    if (handle != INVALID_HANDLE_VALUE)
    { CloseHandle(handle); }
}

function
OS_FUNCTION_GET_FILE_SIZE(win32_get_file_size)
{
    HANDLE handle = (HANDLE)file.u64;
    if (handle == INVALID_HANDLE_VALUE)
    { assert(0); } // TODO: Error-Handling.
    LARGE_INTEGER file_size;
    GetFileSizeEx(handle, &file_size);
    U64 result = file_size.QuadPart;
    return result;
}

function
OS_FUNCTION_READ_FILE(win32_read_file)
{
    HANDLE handle = (HANDLE)file.u64;

    if (handle == INVALID_HANDLE_VALUE)
    { assert(0); }

    LARGE_INTEGER zero_offset = {};
    if (SetFilePointerEx(handle, zero_offset, NULL, FILE_BEGIN) == 0)
    { assert(0); }

    String8 result = {};
    {
        result.str = push_array(arena, U8, size);
        result.count = size;
    }

    if (size > U32_MAX)
    { assert(0); }

    if (!ReadFile(handle, result.str, (DWORD)size, (DWORD *)&result.count, NULL))
    { assert(0); }

    return result;
}

function void
win32_show_message_and_abort(HWND hwnd, LPCWSTR msg)
{
#if 0
    LPCWSTR caption = L"This app is abandoned.";
    UINT message_box_type = MB_ICONSTOP;
    MessageBoxExW(hwnd, msg, caption, message_box_type, 0);
#endif
}

function
OS_FUNCTION_READ_TIMER(win32_read_timer)
{
    LARGE_INTEGER counter;
    QueryPerformanceCounter(&counter);
    U64 result = counter.QuadPart;
    return result;
}

function U64
win32_query_timer_frequency(void)
{
    LARGE_INTEGER frequency;
    QueryPerformanceFrequency(&frequency);
    U64 result = frequency.QuadPart;
    return result;
}

function void
win32_update_dark_mode(HWND hwnd)
{
    static const auto ShouldAppsUseDarkMode = []() {
        const auto uxtheme = LoadLibraryExW(L"uxtheme.dll", nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32);
        return uxtheme ? reinterpret_cast<bool(WINAPI*)()>(GetProcAddress(uxtheme, MAKEINTRESOURCEA(132))) : nullptr;
    }();

    static constexpr auto IsHighContrastOn = []() {
        bool highContrast = false;
        HIGHCONTRAST hc{ sizeof(hc) };
        if (SystemParametersInfoW(SPI_GETHIGHCONTRAST, sizeof(hc), &hc, 0))
        {
            highContrast = (HCF_HIGHCONTRASTON & hc.dwFlags) != 0;
        }
        return highContrast;
    };

    if (ShouldAppsUseDarkMode)
    {
        const BOOL useDarkMode = ShouldAppsUseDarkMode() && !IsHighContrastOn();
        DwmSetWindowAttribute(hwnd, DWMWA_USE_IMMERSIVE_DARK_MODE, &useDarkMode, sizeof(BOOL));
    }
}

#pragma pack(push, 1)
struct Constant_Buffer
{
    V3    color;
    F32   padding;
    M4x4  view_proj;
};
static_assert(sizeof(Constant_Buffer) % 16 == 0);
#pragma pack(pop)


int WINAPI
wWinMain(HINSTANCE hinst, HINSTANCE /*instance_prev_handle*/, PWSTR /*cmdline*/, int /*cmdshow*/)
{
    WNDCLASSW window_class = {};
    {
        window_class.style              = CS_HREDRAW|CS_VREDRAW/*|CS_OWNDC*/;
        window_class.lpfnWndProc        = win32_callback;
        window_class.cbClsExtra         = 0;
        window_class.cbWndExtra         = 0;
        window_class.hInstance          = hinst;
        window_class.hIcon              = LoadIconW(hinst, L"Icon");
        window_class.hCursor            = LoadCursorW(hinst, IDC_ARROW);
        window_class.hbrBackground      = (HBRUSH)GetStockObject(BLACK_BRUSH);
        window_class.lpszMenuName       = NULL;
        window_class.lpszClassName      = L"ClassUnnamed";
    }

    if (!RegisterClassW(&window_class))
    { win32_show_message_and_abort(NULL, L"Failed to register window class."); }

    HWND hwnd = CreateWindowExW(0, window_class.lpszClassName,
                                         L"ILOVEYOU 사랑해요 愛してるよ मुझे तुमसे प्यार है",
                                         WS_OVERLAPPEDWINDOW|WS_VISIBLE,
                                         CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
                                         NULL, NULL, hinst, NULL);
    if (!hwnd) 
    { win32_show_message_and_abort(NULL, L"Failed to create window."); }

    win32_update_dark_mode(hwnd);

    // Note: Initialize OS Functions.
    // TODO: Manually assigning the functions seems unsafe and tedious.
    //        Also, it must be preceded to any OS functionality usage for users..
    {
        os.get_page_size                  = win32_get_page_size;
        os.get_logical_processor_count    = win32_get_logical_processor_count;

        os.abort                          = win32_abort;

        os.reserve                        = win32_reserve;
        os.release                        = win32_release;
        os.commit                         = win32_commit;
        os.decommit                       = win32_decommit;

        os.open_file                      = win32_open_file;
        os.close_file                     = win32_close_file;
        os.get_file_size                  = win32_get_file_size;
        os.read_file                      = win32_read_file;

        os.read_timer                     = win32_read_timer;
        os.timer_frequency                = (F32)win32_query_timer_frequency();
    }

    // Note: Initialize Thread Context.
    thread_init();


    D3D11 d3d11 = {};
    if (!d3d11_init(&d3d11, hwnd))
    { win32_show_message_and_abort(hwnd, L"Failed to initialize D3D11."); }


    // TEMPORARY: Set shader compile flags.
#if BUILD_DEBUG
    UINT compile_flags = D3DCOMPILE_DEBUG|D3DCOMPILE_SKIP_OPTIMIZATION;
#else
    UINT compile_flags = 0;
#endif

    // TEMPORARY: Create Vertex Shader
    ID3DBlob *vertex_shader_blob;
    ID3DBlob *vertex_shader_compile_errors_blob;
    ID3D11VertexShader *vertex_shader;

    String8 shader = read_entire_file(tctx.scratch_arena, string8_literal("shaders/shader.hlsl"));

    if (FAILED(D3DCompile(shader.str, sizeof(shader.str[0])*shader.count, "Simple VS",
                          NULL/*Defines*/, D3D_COMPILE_STANDARD_FILE_INCLUDE, "vs_main",
                          "vs_5_0", compile_flags, 0/*Compile Flags 2*/,
                          &vertex_shader_blob, &vertex_shader_compile_errors_blob)))
    {
        OutputDebugStringA(static_cast<LPCSTR>(vertex_shader_compile_errors_blob->GetBufferPointer()));
        assert(0); 
    }

    if (FAILED(d3d11.device->CreateVertexShader(vertex_shader_blob->GetBufferPointer(), vertex_shader_blob->GetBufferSize(), NULL, &vertex_shader)))
    { assert(0); }



    // TEMPORARY: Create Pixel Shader
    ID3DBlob *pixel_shader_blob;
    ID3DBlob *pixel_shader_compile_errors_blob;
    ID3D11PixelShader *pixel_shader;
    if (FAILED(D3DCompile(shader.str, sizeof(shader.str[0])*shader.count, "Simple PS",
                          NULL/*Defines*/, D3D_COMPILE_STANDARD_FILE_INCLUDE, "ps_main",
                          "ps_5_0", compile_flags, 0/*Compile Flags 2*/,
                          &pixel_shader_blob, &pixel_shader_compile_errors_blob)))
    { assert(0); }

    if (FAILED(d3d11.device->CreatePixelShader(pixel_shader_blob->GetBufferPointer(), pixel_shader_blob->GetBufferSize(), NULL, &pixel_shader)))
    { assert(0); }

    pixel_shader_blob->Release();



    // TEMPORARY: Create Input Layout
    ID3D11InputLayout* input_layout;
    D3D11_INPUT_ELEMENT_DESC input_element_desc[] =
    {
        { "POS", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEX", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
    };

    if (FAILED(d3d11.device->CreateInputLayout(input_element_desc, array_count(input_element_desc), vertex_shader_blob->GetBufferPointer(), vertex_shader_blob->GetBufferSize(), &input_layout)))
    { assert(0); }

    vertex_shader_blob->Release();

    // TEMPORARY: Create Vertex Buffer
    ID3D11Buffer *vertex_buffer;
    U32 vertex_count, stride, offset;
    F32 vertex_data[] = { // x, y, u, v
        -0.5f, +0.5f, 0.f, 0.f,
        +0.5f, -0.5f, 1.f, 1.f,
        -0.5f, -0.5f, 0.f, 1.f,
        -0.5f, +0.5f, 0.f, 0.f,
        +0.5f, +0.5f, 1.f, 0.f,
        +0.5f, -0.5f, 1.f, 1.f
    };
    stride       = 4 * sizeof(F32);
    vertex_count = sizeof(vertex_data) / stride;
    offset       = 0;

    D3D11_BUFFER_DESC vertex_buffer_desc = {};
    {
        vertex_buffer_desc.ByteWidth = sizeof(vertex_data);
        vertex_buffer_desc.Usage     = D3D11_USAGE_IMMUTABLE;
        vertex_buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    }

    D3D11_SUBRESOURCE_DATA vertex_subresource_data = { vertex_data };

    if (FAILED(d3d11.device->CreateBuffer(&vertex_buffer_desc, &vertex_subresource_data, &vertex_buffer)))
    { assert(0); } // TODO: Error-handling


    // TEMPORARY: Create Constant Buffer
    ID3D11Buffer *constant_buffer = NULL;

    D3D11_BUFFER_DESC cb_desc = {};
    {
        cb_desc.ByteWidth      = sizeof(Constant_Buffer);
        cb_desc.Usage          = D3D11_USAGE_DYNAMIC;
        cb_desc.BindFlags      = D3D11_BIND_CONSTANT_BUFFER;
        cb_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    }

    if (FAILED(d3d11.device->CreateBuffer(&cb_desc, NULL, &constant_buffer)))
    { assert(0); }



    // TEMPORARY: Create Sampler State
    D3D11_SAMPLER_DESC sampler_desc = {};
    {
        sampler_desc.Filter         = D3D11_FILTER_MIN_MAG_MIP_POINT;
        sampler_desc.AddressU       = D3D11_TEXTURE_ADDRESS_BORDER;
        sampler_desc.AddressV       = D3D11_TEXTURE_ADDRESS_BORDER;
        sampler_desc.AddressW       = D3D11_TEXTURE_ADDRESS_BORDER;
        sampler_desc.BorderColor[0] = 1.0f;
        sampler_desc.BorderColor[1] = 1.0f;
        sampler_desc.BorderColor[2] = 1.0f;
        sampler_desc.BorderColor[3] = 1.0f;
        sampler_desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    }

    ID3D11SamplerState* sampler_state;
    d3d11.device->CreateSamplerState(&sampler_desc, &sampler_state);

    // TEMPORARY: Load Image, FREE THIS SHSHH
    Bitmap image = image_load(tctx.scratch_arena, string8_literal("doggo.png"));

    // TEMPORARY: Create Texture
    D3D11_TEXTURE2D_DESC texture_desc = {};
    {
        texture_desc.Width              = image.width;
        texture_desc.Height             = image.height;
        texture_desc.MipLevels          = 1;
        texture_desc.ArraySize          = 1;
        texture_desc.SampleDesc.Count   = 1;
        texture_desc.Usage              = D3D11_USAGE_IMMUTABLE;
        texture_desc.BindFlags          = D3D11_BIND_SHADER_RESOURCE;

        // STUDY: SRGB, HDR
        if (image.depth == 8)
        {
            switch (image.channel_count)
            {
                case 1:
                { texture_desc.Format = DXGI_FORMAT_R8_UNORM; } break;
                case 2:
                { texture_desc.Format = DXGI_FORMAT_R8G8_UNORM; } break;
                case 3:
                { assert(0); } break;
                case 4:
                { texture_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; } break;
                default:
                { assert(0); } break;
            }
        }
        else
        {
            switch (image.channel_count)
            {
                case 1:
                { texture_desc.Format = DXGI_FORMAT_R16_UNORM; } break;
                case 2:
                { texture_desc.Format = DXGI_FORMAT_R16G16_UNORM; } break;
                case 3:
                { assert(0); } break;
                case 4:
                { texture_desc.Format = DXGI_FORMAT_R16G16B16A16_UNORM; } break;
                default:
                { assert(0); } break;
            }
        }
    }

    D3D11_SUBRESOURCE_DATA texture_subresource_data = {};
    {
        texture_subresource_data.pSysMem      = image.data;
        texture_subresource_data.SysMemPitch  = image.pitch;
    }

    ID3D11Texture2D *texture;
    d3d11.device->CreateTexture2D(&texture_desc, &texture_subresource_data, &texture);

    ID3D11ShaderResourceView *texture_view;
    d3d11.device->CreateShaderResourceView(texture, NULL, &texture_view);



    // Note: Outer-Loop Variables.
    U64 last_counter = win32_read_timer();

    // Note: Main Loop
    while (global_running)
    {
        // Note: Handle messages
        // STUDY: Why there are two code blocks of handling message? Other's in window callback.
        MSG msg;
        while (PeekMessage(&msg, hwnd, 0, 0, PM_REMOVE))
        {
            switch (msg.message)
            {
                case WM_QUIT: {
                    global_running = false;
                } break;

                default: {
                    TranslateMessage(&msg);
                    DispatchMessageW(&msg);
                } break;
            }
        }

        // Note: Get dt
        U64 counter = win32_read_timer();
        F32 dt = static_cast<F32>(counter - last_counter) / os.timer_frequency;
        last_counter = counter;

        // Note: Update
        local_persist F32 factor = 0.0f;
        factor += dt;
        V3 tint = v3(1.0f, 1.0f, 1.0f);
        tint.g = (F32)cos(factor * 3.0f) * 0.5f + 0.5f;


        // Note: Draw
        FLOAT background_color[4] = { 0.1f, 0.2f, 0.6f, 1.0f };
        d3d11.device_ctx->ClearRenderTargetView(d3d11.framebuffer_view, background_color);

        RECT window_rect;
        GetClientRect(hwnd, &window_rect);
        D3D11_VIEWPORT viewport = {};
        {
            viewport.TopLeftX = 0.0f;
            viewport.TopLeftY = 0.0f;
            viewport.Width    = static_cast<FLOAT>(window_rect.right - window_rect.left);
            viewport.Height   = static_cast<FLOAT>(window_rect.bottom - window_rect.top);
            viewport.MinDepth = 0.0f;
            viewport.MaxDepth = 1.0f;
        }

        d3d11.device_ctx->RSSetViewports(1, &viewport);
        d3d11.device_ctx->OMSetRenderTargets(1, &d3d11.framebuffer_view, NULL/*Depth-Stencil View*/);
        d3d11.device_ctx->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        d3d11.device_ctx->IASetInputLayout(input_layout);

        d3d11.device_ctx->VSSetShader(vertex_shader, NULL, 0);
        d3d11.device_ctx->PSSetShader(pixel_shader, NULL, 0);

        D3D11_MAPPED_SUBRESOURCE mapped_subresource = {};
        d3d11.device_ctx->Map(constant_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_subresource);
        Constant_Buffer *cb = static_cast<Constant_Buffer *>(mapped_subresource.pData);
        {
            cb->color      = tint;
            cb->view_proj  = identity4x4();
        }
        d3d11.device_ctx->Unmap(constant_buffer, 0);


        UINT start_slot   = 0;
        UINT buffer_count = 1;
        d3d11.device_ctx->PSSetConstantBuffers(start_slot, buffer_count, &constant_buffer);


        d3d11.device_ctx->PSSetShaderResources(0, 1, &texture_view);
        d3d11.device_ctx->PSSetSamplers(0, 1, &sampler_state);

        d3d11.device_ctx->IASetVertexBuffers(0, 1, &vertex_buffer, &stride, &offset);

        d3d11.device_ctx->Draw(vertex_count, 0);

        d3d11.swapchain->Present(1, 0);
    }

    return 0;
}
