// Copyright (c) 2025 Seong Woo Lee. All rights reserved.

#define OS_WINDOWS
#include "include/codebase.h"

global B32 g_running = true;

function int
main_entry(Os_Handle hinst)
{
    Temporary_Arena scratch = scratch_begin();
    Os_Handle hwnd = os.create_window(hinst, string16_from_string8(scratch.arena, string8_literal("윈도우 창")));
    scratch_end(scratch);

    while (g_running)
    {
    }

    return 0;
}
