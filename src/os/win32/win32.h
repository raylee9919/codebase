// Copyright (c) 2025 Seong Woo Lee. All rights reserved.

#ifndef LSW_OS_WIN32_H
#define LSW_OS_WIN32_H

#define WM_CREATE_DANGEROUS_WINDOW  (WM_USER + 0x1337)
#define WM_DESTROY_DANGEROUS_WINDOW (WM_USER + 0x1338)

typedef struct Win32_Thread Win32_Thread;
struct Win32_Thread
{
    HANDLE          handle;
    Os_Thread_Proc  *proc;
    void            *param;
};

typedef struct
{
    wchar_t *title;
    U32 width;
    U32 height;
} Win32_Create_Window_Param;

typedef struct Win32_State
{
    Arena           *arena;

    HINSTANCE       hinst;
    WNDCLASSEXW     wcex;
    HWND            service_window;
    DWORD           service_thread_id;
    DWORD           main_thread_id;
} Win32_State;
global Win32_State win32;










#endif // LSW_OS_WIN32_H
