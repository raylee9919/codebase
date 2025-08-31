// Copyright (c) 2025 Seong Woo Lee. All rights reserved.

/* 
NOTE:
1. channel sequence
2. bytes per pixel
*/

function Bitmap
image_load(Arena *arena, String8 file_path)
{
    Temporary_Arena scratch = scratch_begin();

    String8 loaded = read_entire_file(scratch.arena, file_path);

    U8 *data;
    S32 width, height, channel_count, bytes_per_channel;
    S32 size = (S32)loaded.count * sizeof(loaded.str[0]);

    if (stbi_is_16_bit_from_memory(loaded.str, size))
    {
        data = (U8 *)stbi_load_16_from_memory(loaded.str, size, &width, &height, &channel_count, 0);
        bytes_per_channel = 16;
    }
    else
    {
        data = stbi_load_from_memory(loaded.str, size, &width, &height, &channel_count, 0);
        bytes_per_channel = 8;
    }

    if (! data)
    { assert(0); }

    Bitmap result = {};
    {
        result.channel_count     = channel_count;
        result.bytes_per_channel = bytes_per_channel;
        result.width             = width;
        result.height            = height;
        result.pitch             = width*bytes_per_channel*channel_count;
        result.data              = data;
    }

    scratch_end(scratch);
    return result;
}
