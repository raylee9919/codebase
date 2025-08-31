// Copyright (c) 2025 Seong Woo Lee. All rights reserved.

function String8
read_entire_file(Arena *arena, String8 file_path)
{
    Os_Handle handle = os.open_file(file_path, OS_FILE_ACCESS_READ);
    U64 file_size = os.get_file_size(handle);
    String8 result = os.read_file(arena, handle, file_size);
    os.close_file(handle);
    return result;
}
