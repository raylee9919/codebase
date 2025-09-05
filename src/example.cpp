// Copyright (c) 2025 Seong Woo Lee. All rights reserved.

#define OS_WINDOWS
#include "include/codebase.h"

global B32 g_running = true;

function int
main_entry(void)
{
    Arena *arena = arena_alloc();

    Dynamic_Array(arena, int) d = {};
    dar_push(&d, 1);
    dar_reserve(&d, 65);

    F64 inv_timer_freq = 1.0 / (F64)os_query_timer_frequency();
    U64 old_counter = os_read_timer();
    F32 time = 0.0;
    
    Os_Window *window = os_create_window(1920, 1080, L"Main Window");

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

        time += dt;

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
