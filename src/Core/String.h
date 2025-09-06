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

typedef struct Unicode_Decode Unicode_Decode;
struct Unicode_Decode
{
    U32 inc;
    U32 codepoint;
};

global U8 utf8_class[32] = {
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,2,2,2,2,3,3,4,5,
};

// Note: C-String
function U64 cstring_length(char *cstr);

// Note: Helper Functions.
function B32 is_alpha(U8 c);
function B32 is_digit(U8 c);
function B32 is_whitespace(U8 c);

// Note: Constructors.
#define utf8_literal(str) utf8((U8 *)str, sizeof(str) - 1)
function Utf8 utf8(U8 *str, U64 count);
function Utf16 utf16(U16 *str, U64 count);
function Utf32 utf32(U32 *str, U64 count);

// Note: Encoding/Decoding.
function Unicode_Decode utf8_decode(U8 *str, U64 max);
function Unicode_Decode utf16_decode(U16 *str, U64 max);
function U32 utf8_encode(U8 *str, U32 codepoint);
function U32 utf16_encode(U16 *str, U32 codepoint);

// Note: Conversion.
function Utf8 utf8_from_utf16(Arena *arena, Utf16 in);
function Utf16 utf16_from_utf8(Arena *arena, Utf8 in);


#endif // LSW_STRING_H
