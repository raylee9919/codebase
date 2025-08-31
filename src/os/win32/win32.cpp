// Copyright (c) 2025 Seong Woo Lee. All rights reserved.
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#define UNICODE
#define _UNICODE
#include <windows.h>

function LRESULT CALLBACK
win32_callback(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
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
OS_SHOW_MESSAGE(win32_show_message)
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

    String16 path16 = string16_from_string8(scratch.arena, path);

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

    String8 result = {};
    {
        result.str = push_array(arena, U8, size);
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

function void
win32_show_message_and_abort(HWND hwnd, LPCWSTR msg)
{
#if 0
    LPCWSTR caption = L"This app is abandoned.";
    UINT message_box_type = MB_ICONSTOP;
    MessageBoxExW(hwnd, msg, caption, message_box_type, 0);
#endif
}

function U64
win32_query_timer_frequency(void)
{
    LARGE_INTEGER frequency;
    QueryPerformanceFrequency(&frequency);
    U64 result = frequency.QuadPart;
    return result;
}

function void
win32_init(void)
{
    os.get_page_size                  = win32_get_page_size;
    os.get_logical_processor_count    = win32_get_logical_processor_count;
    os.abort                          = win32_abort;
    os.reserve                        = win32_reserve;
    os.release                        = win32_release;
    os.commit                         = win32_commit;
    os.decommit                       = win32_decommit;
    os.open_file                      = win32_open_file;
    os.close_file                     = win32_close_file;
    os.get_file_size                  = win32_get_file_size;
    os.read_file                      = win32_read_file;
    os.read_timer                     = win32_read_timer;
    os.timer_frequency                = (F32)win32_query_timer_frequency();
}
