// Copyright (c) 2025 Seong Woo Lee. All rights reserved.
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#define UNICODE
#define _UNICODE
#include <windows.h>

#pragma comment(lib, "kernel32")
#pragma comment(lib, "user32")

LRESULT CALLBACK
win32_window_proc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    LRESULT result = {};

    switch(msg) 
    {
        case WM_DESTROY: {
            PostQuitMessage(0);
        } break;

        default: {
            result = DefWindowProcW(hwnd, msg, wparam, lparam);
        } break;
    }

    return result;
}

function
OS_CREATE_WINDOW(win32_create_window)
{
    Os_Handle result = {};
    Temporary_Arena scratch = scratch_begin();

    // @Note: Place this before creating window.
    // win32_assume_hr(SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2));
    
    HINSTANCE hinst = win32.hinst;
    assert(hinst);

    char buf[256];
    stbsp_sprintf(buf, "wcex%d", win32.next_window_idx);
    U64 buf_len = cstring_length(buf);
    Utf8 wcex_name_8 = utf8((U8 *)buf, buf_len);
    Utf16 wcex_name_16 = utf16_from_utf8(scratch.arena, wcex_name_8);

    WNDCLASSW wcex = {};
    {
        wcex.style              = CS_HREDRAW|CS_VREDRAW|CS_OWNDC;
        wcex.lpfnWndProc        = win32_window_proc;
        wcex.cbClsExtra         = 0;
        wcex.cbWndExtra         = 0;
        wcex.hInstance          = hinst;
        wcex.hIcon              = LoadIcon(hinst, IDI_APPLICATION);
        wcex.hCursor            = LoadCursor(hinst, IDC_ARROW);
        wcex.hbrBackground      = (HBRUSH)GetStockObject(BLACK_BRUSH);
        wcex.lpszMenuName       = NULL;
        wcex.lpszClassName      = (wchar_t *)wcex_name_16.str;
    }

    if (RegisterClassW(&wcex))
    {
        HWND hwnd = CreateWindowExW(0/*style->DWORD*/, wcex.lpszClassName, title,
                                    WS_OVERLAPPEDWINDOW|WS_VISIBLE,
                                    CW_USEDEFAULT, CW_USEDEFAULT, width, height,
                                    NULL, NULL, hinst, NULL);
        result.u64 = (U64)hwnd;
    }

    ++win32.next_window_idx;

    scratch_end(scratch);
    return result;
}

function
OS_GET_CLIENT_SIZE(win32_get_client_size)
{
    V2U result = {};
    HWND hwnd = (HWND)window.u64;
    RECT rect = {};
    GetClientRect(hwnd, &rect);
    result.x = rect.right - rect.left;
    result.y = rect.bottom - rect.top;
    return result;
}

function DWORD WINAPI
win32_thread_proc(void *param)
{
    Win32_Thread *thread = (Win32_Thread *)param;
    thread->proc(thread->param);
    return 0;
}

function
OS_CREATE_THREAD(win32_create_thread)
{
    Os_Handle result = {};

    DWORD thread_id;

    Win32_Thread *thread = arena_push_struct(win32.arena, Win32_Thread);
    thread->proc   = proc;
    thread->param  = param;
    thread->handle = CreateThread(0, 0, win32_thread_proc, thread, 0, &thread_id);

    result.u64 = (U64)thread->handle;

    return result;
}

function
OS_JOIN_THREAD(win32_join_thread)
{
    HANDLE hthread = (HANDLE)thread.u64;
    if (hthread)
    {
        WaitForSingleObject(hthread, INFINITE);
        CloseHandle(hthread);
    }
}

function
OS_GUI_MESSAGE(win32_gui_message)
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
OS_GET_PAGE_SIZE(win32_get_page_size)
{
    SYSTEM_INFO info;
    GetSystemInfo(&info);
    return info.dwPageSize;
}

function
OS_GET_LOGICAL_PROCESSOR_COUNT(win32_get_logical_processor_count)
{
    SYSTEM_INFO info;
    GetSystemInfo(&info);
    U32 result = info.dwNumberOfProcessors;
    return result;
}

// Note: Abort
function
OS_ABORT(win32_abort)
{
    ExitProcess(1);
}

//
// Note: Memory
//
function
OS_RESERVE(win32_reserve)
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
OS_RELEASE(win32_release)
{
    VirtualFree(ptr, 0, MEM_RELEASE);
}

function
OS_COMMIT(win32_commit)
{
    U64 page_snapped_size = size;
    page_snapped_size += win32_get_page_size() - 1;
    page_snapped_size -= page_snapped_size % win32_get_page_size();
    VirtualAlloc(ptr, page_snapped_size, MEM_COMMIT, PAGE_READWRITE);
}

function
OS_DECOMMIT(win32_decommit)
{
    VirtualFree(ptr, size, MEM_DECOMMIT);
}

//
// Note: File System
//
function
OS_OPEN_FILE(win32_open_file)
{
    Temporary_Arena scratch = scratch_begin();

    Utf16 path16 = utf16_from_utf8(scratch.arena, path);

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
OS_CLOSE_FILE(win32_close_file)
{
    HANDLE handle = (HANDLE)file.u64;
    if (handle != INVALID_HANDLE_VALUE)
    { CloseHandle(handle); }
}

function
OS_GET_FILE_SIZE(win32_get_file_size)
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
OS_READ_FILE(win32_read_file)
{
    HANDLE handle = (HANDLE)file.u64;

    if (handle == INVALID_HANDLE_VALUE)
    { assert(0); }

    LARGE_INTEGER zero_offset = {};
    if (SetFilePointerEx(handle, zero_offset, NULL, FILE_BEGIN) == 0)
    { assert(0); }

    Utf8 result = {};
    {
        result.str = arena_push_array(arena, U8, size);
        result.count = size;
    }

    if (size > U32_MAX)
    { assert(0); }

    if (!ReadFile(handle, result.str, (DWORD)size, (DWORD *)&result.count, NULL))
    { assert(0); }

    return result;
}

function
OS_READ_TIMER(win32_read_timer)
{
    LARGE_INTEGER counter;
    QueryPerformanceCounter(&counter);
    U64 result = counter.QuadPart;
    return result;
}

function
OS_QUERY_TIMER_FREQUENCY(win32_query_timer_frequency)
{
    LARGE_INTEGER frequency;
    QueryPerformanceFrequency(&frequency);
    U64 result = frequency.QuadPart;
    return result;
}

function void
win32_init(HINSTANCE hinst)
{
    os_create_window                  = win32_create_window;
    os_get_client_size                = win32_get_client_size;
    os_create_thread                  = win32_create_thread;
    os_join_thread                    = win32_join_thread;
    os_get_page_size                  = win32_get_page_size;
    os_get_logical_processor_count    = win32_get_logical_processor_count;
    os_gui_message                    = win32_gui_message;
    os_abort                          = win32_abort;
    os_reserve                        = win32_reserve;
    os_release                        = win32_release;
    os_commit                         = win32_commit;
    os_decommit                       = win32_decommit;
    os_open_file                      = win32_open_file;
    os_close_file                     = win32_close_file;
    os_get_file_size                  = win32_get_file_size;
    os_read_file                      = win32_read_file;
    os_read_timer                     = win32_read_timer;
    os_query_timer_frequency          = win32_query_timer_frequency;

    win32.arena = arena_alloc();
    win32.hinst = hinst;
}
