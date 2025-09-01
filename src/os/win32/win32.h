// Copyright (c) 2025 Seong Woo Lee. All rights reserved.
#ifndef LSW_OS_WIN32_H
#define LSW_OS_WIN32_H

typedef struct Win32_Thread Win32_Thread;
struct Win32_Thread
{
    HANDLE          handle;
    Os_Thread_Proc  *proc;
    void            *param;
};

typedef struct Win32_State Win32_State;
struct Win32_State
{
    Arena *arena;

    HINSTANCE hinst;
    int next_window_idx;
};
global Win32_State win32;










#endif // LSW_OS_WIN32_H
