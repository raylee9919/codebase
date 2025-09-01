// Copyright (c) 2025 Seong Woo Lee. All rights reserved.

#ifndef LSW_STRING_H
#define LSW_SRRING_H

#define STB_SPRINTF_IMPLEMENTATION
#include "core/vendor/stb_sprintf.h"

typedef struct Utf8 Utf8;
struct Utf8 
{
    U8 *str;
    U64 count;
};

typedef struct Utf16 Utf16;
struct Utf16 
{
    U16 *str;
    U64 count;
};

typedef struct Utf32 Utf32;
struct Utf32 
{
    U32 *str;
    U64 count;
};

// @Note: Fancy way to say integer. Equivalent to 21-bits.
typedef struct Codepoint Codepoint;
struct Codepoint
{
    U32 val;
    U32 advance;
};

// Note: C-String
function U64 cstring_length(char *cstr);

// Note: Helper Functions.
function B32 is_alpha(U8 c);
function B32 is_digit(U8 c);
function B32 is_whitespace(U8 c);

// Note: Constructors.
#define utf8_literal(str) string8((U8 *)str, sizeof(str) - 1)
function Utf8 utf8(U8 *str, U64 count);
function Utf16 utf16(U16 *str, U64 count);
function Utf32 utf32(U32 *str, U64 count);

// Note: Encoding/Decoding.
function Codepoint codepoint_from_utf8(U8 *str, U64 max);
function U32 utf16_from_codepoint(U16 *out, U32 codepoint);

// Note: Conversion.
function Utf16 utf16_from_utf8(Arena *arena, Utf8 str8);


#endif // LSW_STRING_H
