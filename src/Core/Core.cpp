// Copyright (c) 2025 Seong Woo Lee. All rights reserved.

function U16
safe_u16_from_u32(U32 x)
{
    assert(x <= U16_MAX);
    U16 result = (U16)x;
    return result;
}

function U32
safe_u32_from_u64(U64 x)
{
    assert(x <= U32_MAX);
    U32 result = (U32)x;
    return result;
}

function S32
safe_s32_from_s64(S64 x)
{
    assert(x <= S32_MAX);
    S32 result = (S32)x;
    return result;
}

function Utf8
read_entire_file(Arena *arena, Utf8 file_path)
{
    Os_Handle handle = os_open_file(file_path, OS_FILE_ACCESS_READ);
    U64 file_size = os_get_file_size(handle);
    Utf8 result = os_read_file(arena, handle, file_size);
    os_close_file(handle);
    return result;
}
