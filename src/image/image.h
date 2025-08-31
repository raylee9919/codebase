// Copyright (c) 2025 Seong Woo Lee. All rights reserved.

#ifndef LSW_IMAGE_H
#define LSW_IMAGE_H

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ASSERT(x)
#include "image/stb_image.h"

typedef struct Bitmap Bitmap;
struct Bitmap
{
    U32 width;
    U32 height;
    U32 pitch;
    U32 size;
    U8 *data;
};


function Bitmap image_load(Arena *arena, String8 file_path);


#endif // LSW_IMAGE_H
