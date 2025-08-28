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
    S32 width, height, channel_count, bpp, depth;
    S32 size = (S32)loaded.count * sizeof(loaded.str[0]);

    if (stbi_is_16_bit_from_memory(loaded.str, size))
    {
        data = (U8 *)stbi_load_16_from_memory(loaded.str, size, &width, &height, &channel_count, 0);
        bpp = (channel_count << 1);
        depth = 16;
    }
    else
    {
        data = stbi_load_from_memory(loaded.str, size, &width, &height, &channel_count, 0);
        bpp = channel_count;
        depth = 8;
    }

    if (! data)
    { assert(0); }

    Bitmap result = {};
    {
        result.data           = data;
        result.width          = width;
        result.height         = height;
        result.pitch          = width * bpp;
        result.depth          = depth;
        result.channel_count  = channel_count;
        result.bpp            = bpp;
    }

    scratch_end(scratch);
    return result;
}
