// Copyright (c) 2025 Seong Woo Lee. All rights reserved.

// ---------------------------------
// @Note: Actual entry for windows.

#ifdef OS_WINDOWS
#  include "os/win32/win32.cpp"
#  ifdef BUILD_CLI
    int main(int argc, char **argv)
    {
        win32_init();
        thread_main_init();
        main_entry(argc, argv);
    }
#  else
    int WINAPI wWinMain(HINSTANCE hinst, HINSTANCE hinst_prev, PWSTR cmdline, int cmdshow)
    {
        win32_init();
        thread_main_init();

        Os_Handle instance = {};
        instance.u64 = (U64)hinst;
        main_entry(instance);
    }
#  endif
#else
#  error Define OS: OS_WINDOWS|OS_LINUX|OS_MAC
#endif
