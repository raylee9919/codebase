// Copyright (c) 2025 Seong Woo Lee. All rights reserved.

#define OS_WINDOWS
#include "include/codebase.h"

function int
main_entry(Os_Handle hinst)
{
    Os_Handle hwnd = os.create_window(hinst);
    return 0;
}
