// Copyright (c) 2025 Seong Woo Lee. All rights reserved.

#ifdef OS_WINDOWS
#  include "os/win32/win32.h"
#  include "os/win32/win32.cpp"
#  ifdef BUILD_CLI
    int main(int argc, char **argv)
    {
        win32_init();
        thread_init();
        main_entry(argc, argv);

        return 0;
    }
#  else
    int WINAPI wWinMain(HINSTANCE hinst, HINSTANCE hinst_prev, PWSTR cmdline, int cmdshow)
    {
        win32_init(hinst);
        thread_init();

        win32.service_thread_id = GetCurrentThreadId();

        WNDCLASSEXW wcex = {};
        {
            wcex.cbSize             = sizeof(wcex);
            wcex.lpfnWndProc        = &win32_service_window_proc;
            wcex.hInstance          = GetModuleHandle(NULL);
            wcex.hIcon              = LoadIcon(NULL, IDI_APPLICATION);
            wcex.hCursor            = LoadCursor(NULL, IDC_ARROW);
            wcex.hbrBackground      = (HBRUSH)GetStockObject(BLACK_BRUSH);
            wcex.lpszClassName      = L"ServiceWindowClass";
        }

        if (! RegisterClassExW(&wcex))
        { assume(0); }

        win32.service_window = CreateWindowExW(0, wcex.lpszClassName, L"ServiceWindow", 0,
                                               CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
                                               0, 0, wcex.hInstance, 0);
        assume(win32.service_window);

        CreateThread(0, 0, win32_gui_main_thread_entry, 0, 0, &win32.main_thread_id);

        for (;;)
        {
            MSG msg;
            GetMessageW(&msg, 0, 0, 0);
            TranslateMessage(&msg);

            if ( (msg.message == WM_CHAR) || (msg.message == WM_KEYDOWN) ||
                 (msg.message == WM_QUIT) || (msg.message == WM_SIZE) ) 
            {
                char buf[256];
                snprintf(buf, 256, "Service Thread: %d\n", msg.message);
                OutputDebugString(buf);

                PostThreadMessageW(win32.main_thread_id, msg.message, msg.wParam, msg.lParam);
            }
            else
            {
                DispatchMessageW(&msg);
            }
        }

        // @Note: Main thread will call ExitProcess().
    }
#  endif
#else
#  error Define OS: OS_WINDOWS|OS_LINUX|OS_MAC
#endif
