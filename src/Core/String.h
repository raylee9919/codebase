// Copyright (c) 2025 Seong Woo Lee. All rights reserved.

#ifndef LSW_STRING_H
#define LSW_SRRING_H

typedef struct String8 String8;
struct String8 
{
    U8 *str;
    U64 count;
};

typedef struct String16 String16;
struct String16 
{
    U16 *str;
    U64 count;
};

typedef struct String32 String32;
struct String32 
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

// @Note: Helper Functions.
function B32 is_alpha(U8 c);
function B32 is_digit(U8 c);
function B32 is_whitespace(U8 c);
function U64 length_cstring(char *cstr);
function B32 string8_equal(U8 *a, U8 *b, U64 len);

// @Note: Constructors.
#define string8_literal(str) string8((U8 *)str, sizeof(str) - 1)
function String8 string8(U8 *str, U64 count);
function String16 string16(U16 *str, U64 count);
function String32 string32(U32 *str, U64 count);

// @Note: Encoding/Decoding.
function Codepoint codepoint_from_utf8(U8 *str, U64 max);
function U32 utf16_from_codepoint(U16 *out, U32 codepoint);

// @Note: Conversion.
function String16 string16_from_string8(Arena *arena, String8 str8);


#endif // LSW_STRING_H
