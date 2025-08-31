// Copyright (c) 2025 Seong Woo Lee. All rights reserved.

#define OS_WINDOWS
#include "include/codebase.h"

function int
main_entry(Os_Handle hinst)
{
    Temporary_Arena scratch = scratch_begin();
    Os_Handle hwnd = os.create_window(hinst, string16_from_string8(scratch.arena, string8_literal("안녕하세요")));
    scratch_end(scratch);

    for (;;)
    {
    }

    return 0;
}
