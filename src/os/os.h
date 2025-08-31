// Copyright (c) 2025 Seong Woo Lee. All rights reserved.
#ifndef LSW_OS_H
#define LSW_OS_H

struct String8;
struct String16;
struct Arena;

// @Note: Abstracted OS Types.
typedef struct Os_Handle Os_Handle;
struct Os_Handle
{
    U64 u64;
};

typedef U32 Os_File_Access_Flags;
enum
{
    OS_FILE_ACCESS_READ       = (1<<0),
    OS_FILE_ACCESS_WRITE      = (1<<1),
    OS_FILE_ACCESS_SHARED     = (1<<2),
    OS_FILE_ACCESS_CREATE_NEW = (1<<3),
};

// @Note: OS Macro Magic.
#define OS_FUNCTION_MAIN_ENTRY(name)                  int name(void)
#define OS_FUNCTION_GET_PAGE_SIZE(name)               U64 name(void)
#define OS_FUNCTION_GET_LOGICAL_PROCESSOR_COUNT(name) U32 name(void)
#define OS_FUNCTION_SHOW_MESSAGE(name)                void name(String16 msg)
#define OS_FUNCTION_ABORT(name)                       void name(void)
#define OS_FUNCTION_RESERVE(name)                     void *name(U64 size, B32 commit)
#define OS_FUNCTION_RELEASE(name)                     void name(void *ptr)
#define OS_FUNCTION_COMMIT(name)                      void name(void *ptr, U64 size)
#define OS_FUNCTION_DECOMMIT(name)                    void name(void *ptr, U64 size)
#define OS_FUNCTION_OPEN_FILE(name)                   Os_Handle name(String8 path, Os_File_Access_Flags flags)
#define OS_FUNCTION_CLOSE_FILE(name)                  void name(Os_Handle file)
#define OS_FUNCTION_GET_FILE_SIZE(name)               U64 name(Os_Handle file)
#define OS_FUNCTION_READ_FILE(name)                   String8 name(Arena *arena, Os_Handle file, U64 size)
#define OS_FUNCTION_READ_TIMER(name)                  U64 name(void)

// @Note: OS Typedef Magic.
typedef OS_FUNCTION_GET_PAGE_SIZE(Os_Get_Page_Size);
typedef OS_FUNCTION_GET_LOGICAL_PROCESSOR_COUNT(Os_Get_Logical_Processor_Count);
typedef OS_FUNCTION_SHOW_MESSAGE(Os_Show_Message);
typedef OS_FUNCTION_ABORT(Os_Abort);
typedef OS_FUNCTION_RESERVE(Os_Reserve);
typedef OS_FUNCTION_RELEASE(Os_Release);
typedef OS_FUNCTION_COMMIT(Os_Commit);
typedef OS_FUNCTION_DECOMMIT(Os_Decommit);
typedef OS_FUNCTION_OPEN_FILE(Os_Open_File);
typedef OS_FUNCTION_CLOSE_FILE(Os_Close_File);
typedef OS_FUNCTION_GET_FILE_SIZE(Os_Get_File_Size);
typedef OS_FUNCTION_READ_FILE(Os_Read_File);
typedef OS_FUNCTION_READ_TIMER(Os_Read_Timer);


typedef struct Os_State Os_State;
struct Os_State
{
    Os_Get_Page_Size                *get_page_size;
    Os_Get_Logical_Processor_Count  *get_logical_processor_count;

    Os_Show_Message                 *show_message;
    Os_Abort                        *abort;

    Os_Reserve                      *reserve;
    Os_Release                      *release;
    Os_Commit                       *commit;
    Os_Decommit                     *decommit;

    Os_Open_File                    *open_file;
    Os_Close_File                   *close_file;
    Os_Get_File_Size                *get_file_size;
    Os_Read_File                    *read_file;

    Os_Read_Timer                   *read_timer;
    F32 timer_frequency;
};

global Os_State os;


// ------------------------
// @Note: Per-OS Entry.
function int main_entry(void); // User code space forward declaration.

#if OS_WINDOWS
#  include "os/win32/win32.cpp"
#  if BUILD_CLI
   int main(int argc, char **argv)
   {
       win32_init();
       main_entry();
   }
#  else
   int WINAPI wWinMain(HINSTANCE hinst, HINSTANCE hinst_prev, PWSTR cmdline, int cmdshow)
   {
       win32_init();
       main_entry();
   }
#  endif
#else
#  error Define OS: OS_WINDOWS|OS_LINUX|OS_MAC
#endif



#endif // LSW_OS_H
