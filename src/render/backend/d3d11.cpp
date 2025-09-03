// Copyright (c) 2025 Seong Woo Lee. All rights reserved.

function B32
d3d11_init(HWND window_handle)
{
    // NOTE: Init D3D11 device and context.
    ID3D11Device *base_device;
    ID3D11DeviceContext *base_device_ctx;

    D3D_FEATURE_LEVEL desired_feature_level = D3D_FEATURE_LEVEL_11_0;
    D3D_FEATURE_LEVEL feature_levels[] = { desired_feature_level };
    D3D_FEATURE_LEVEL feature_level;

    UINT creation_flags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#if BUILD_DEBUG
    creation_flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    if (FAILED(D3D11CreateDevice(NULL, D3D_DRIVER_TYPE_HARDWARE,
                                 NULL, creation_flags,
                                 feature_levels, array_count(feature_levels),
                                 D3D11_SDK_VERSION, &base_device,
                                 &feature_level, &base_device_ctx)))
    { return false; }

    // NOTE: Query interfaces from device and device-context.
    if (FAILED(base_device->QueryInterface(__uuidof(ID3D11Device1), (void **)&d3d11.device)))
    { return false; }

    base_device->Release();

    if (FAILED(base_device_ctx->QueryInterface(__uuidof(ID3D11DeviceContext1), (void**)&d3d11.device_ctx)))
    { return false; }

    base_device_ctx->Release();

    // NOTE: Set up debug layer to break on D3D11 errors.
#if BUILD_DEBUG
    ID3D11Debug *d3d11_debug = NULL;
    d3d11.device->QueryInterface(__uuidof(ID3D11Debug), (void**)&d3d11_debug);
    if (!d3d11_debug)
    { return false; }
    ID3D11InfoQueue *d3d11_info_queue = NULL;
    if (FAILED(d3d11_debug->QueryInterface(__uuidof(ID3D11InfoQueue), (void**)&d3d11_info_queue)))
    { return false; }
    d3d11_info_queue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_CORRUPTION, true);
    d3d11_info_queue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_ERROR, true);
    d3d11_info_queue->Release();
    d3d11_debug->Release();
#endif

    // NOTE: Create swapchain and framebuffer.
    if (!d3d11_create_swapchain_and_framebuffer(window_handle))
    { return false; }

    return true;
}

function B32
d3d11_create_swapchain_and_framebuffer(HWND window_handle)
{
    // NOTE: Create Swapchain
    IDXGIFactory2 *dxgi_factory;
    IDXGIDevice1 *dxgi_device;
    if (FAILED(d3d11.device->QueryInterface(__uuidof(IDXGIDevice1), (void**)&dxgi_device)))
    { return false; }

    IDXGIAdapter *dxgi_adapter;
    if (FAILED(dxgi_device->GetAdapter(&dxgi_adapter)))
    { return false; }

    dxgi_device->Release();

    DXGI_ADAPTER_DESC adapter_desc;
    dxgi_adapter->GetDesc(&adapter_desc);

    // OutputDebugStringW(L"Graphics Device: ");
    // OutputDebugStringW(adapterDesc.Description);

    if (FAILED(dxgi_adapter->GetParent(__uuidof(IDXGIFactory2), (void**)&dxgi_factory)))
    { return false; }

    dxgi_adapter->Release();

    DXGI_SWAP_CHAIN_DESC1 d3d11_swapchain_desc = {};
    {
        d3d11_swapchain_desc.Width              = 0; // Use window width and height.
        d3d11_swapchain_desc.Height             = 0;
        d3d11_swapchain_desc.Format             = DXGI_FORMAT_B8G8R8A8_UNORM;
        d3d11_swapchain_desc.SampleDesc.Count   = 1;
        d3d11_swapchain_desc.SampleDesc.Quality = 0;
        d3d11_swapchain_desc.BufferUsage        = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        d3d11_swapchain_desc.BufferCount        = 2;
        d3d11_swapchain_desc.Scaling            = DXGI_SCALING_STRETCH;
        d3d11_swapchain_desc.SwapEffect         = DXGI_SWAP_EFFECT_DISCARD;
        d3d11_swapchain_desc.AlphaMode          = DXGI_ALPHA_MODE_UNSPECIFIED;
        d3d11_swapchain_desc.Flags              = 0;
    }

    if (FAILED(dxgi_factory->CreateSwapChainForHwnd(d3d11.device, window_handle, &d3d11_swapchain_desc, 0, 0, &d3d11.swapchain)))
    { return false; }

    dxgi_factory->Release();

    // NOTE: Create framebuffer render target
    ID3D11Texture2D *d3d11_framebuffer;

    if (FAILED(d3d11.swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void **)&d3d11_framebuffer)))
    { return false; }

    if (FAILED(d3d11.device->CreateRenderTargetView(d3d11_framebuffer, 0, &d3d11.framebuffer_view)))
    { return false; }

    d3d11_framebuffer->Release();

    return true;
}
