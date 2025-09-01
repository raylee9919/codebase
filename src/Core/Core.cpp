// Copyright (c) 2025 Seong Woo Lee. All rights reserved.

function Utf8
read_entire_file(Arena *arena, Utf8 file_path)
{
    Os_Handle handle = os_open_file(file_path, OS_FILE_ACCESS_READ);
    U64 file_size = os_get_file_size(handle);
    Utf8 result = os_read_file(arena, handle, file_size);
    os_close_file(handle);
    return result;
}
