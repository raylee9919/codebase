// Copyright (c) 2025 Seong Woo Lee. All rights reserved.

#define OS_WINDOWS
#include "include/codebase.h"

global B32 g_running = true;

function int
main_entry(void)
{
    Arena *arena = arena_alloc();

    F64 inv_timer_freq = 1.0 / (F64)os_query_timer_frequency();
    U64 old_counter = os_read_timer();
    F32 time = 0.0;
    
    Os_Window *window = os_create_window(960, 540, L"Main Window");

    while (!window->should_close)
    {
        for (MSG msg; PeekMessage(&msg, 0, 0, 0, PM_REMOVE);)
        {
            switch (msg.message)
            {
                case WM_CHAR: {
                    os_create_window(900, 600, L"Child Window");
                } break;

            }
        }

        U64 new_counter = os_read_timer();
        F32 dt = (F32)((F64)(new_counter - old_counter) * inv_timer_freq);
        old_counter = new_counter;

        // ---------------------------------
        // @Note: Update

        time += dt;

        U32 event_count = os.event_count;
        for (U32 i = 0; i < event_count; ++i)
        {
            Os_Event event = os.event_queue[i];
            --os.event_count;

            if (event.type == OS_EVENT_DRAG_AND_DROP)
            {
                for (U32 fi = 0; fi < event.file_count; ++fi)
                {
                    Utf16 file_path = event.file_paths[fi];
                    OutputDebugStringW((wchar_t *)file_path.str);
                    OutputDebugStringW(L"\n");
                }
            }
            else
            {
                assert(! "Unknown Event");
            }
        }

        dll_for (os.window_sentinel, it)
        {
            HWND hwnd = (HWND)it->handle.u64;
            HDC dc = GetDC(hwnd);
            V2U dim = os_get_client_size(it);
            F32 n = sinf(time)*0.5f + 0.5f;
            int x = (int)(n*dim.x);
            PatBlt(dc, 0, 0, x, dim.y, BLACKNESS);
            PatBlt(dc, x, 0, dim.x, dim.y, WHITENESS);
            ReleaseDC(hwnd, dc);
        }

        dll_for (os.window_sentinel, it)
        {
            if (it->should_close)
            {
                os_close_window(it);
            }
        }
    }

    return 0;
}
