// Copyright (c) 2025 Seong Woo Lee. All rights reserved.


// ------------------------------
// Note: C-String.
//
function U64
cstring_length(char *cstr)
{
    U64 result = 0;
    for(;cstr[result]; result++);
    return result;
}

// ------------------------------
// Note: Helper Functions.
//
function B32
is_alpha(U8 c)
{
    U8 masked = (c & (~32));
    B32 result = (masked >= 65 && masked <= 90);
    return result;
}

function B32
is_digit(U8 c)
{
    B32 result = (c >= 48 && c <= 57);
    return result;
}

function B32
is_whitespace(U8 c)
{
    B32 result = (c == ' '  || c == '\t' || c == '\r' ||
                  c == '\n' || c == '\f' || c == '\v');
    return result;
}

//
// Note: String Constructors
//
function Utf8
utf8(U8 *str, U64 count)
{
    Utf8 result = {};
    result.str = str;
    result.count = count;
    return result;
}

function Utf16
utf16(U16 *str, U64 count)
{
    Utf16 result = {};
    result.str = str;
    result.count = count;
    return result;
}

function Utf32
utf32(U32 *str, U64 count)
{
    Utf32 result = {};
    result.str = str;
    result.count = count;
    return result;
}

//
// Note: Encoding/Decoding.
//
function Unicode_Decode
utf8_decode(U8 *str, U64 max)
{
    Unicode_Decode result = {1, U32_MAX};
    U8 byte = str[0];
    U8 byte_class = utf8_class[byte >> 3];
    switch (byte_class)
    {
        case 1: {
            result.codepoint = byte;
        } break;
        case 2: {
            if (1 < max)
            {
                U8 cont_byte = str[1];
                if (utf8_class[cont_byte >> 3] == 0)
                {
                    result.codepoint = (byte & bitmask5) << 6;
                    result.codepoint |=  (cont_byte & bitmask6);
                    result.inc = 2;
                }
            }
        } break;
        case 3: {
            if (2 < max)
            {
                U8 cont_byte[2] = {str[1], str[2]};
                if (utf8_class[cont_byte[0] >> 3] == 0 &&
                    utf8_class[cont_byte[1] >> 3] == 0)
                {
                    result.codepoint = (byte & bitmask4) << 12;
                    result.codepoint |= ((cont_byte[0] & bitmask6) << 6);
                    result.codepoint |=  (cont_byte[1] & bitmask6);
                    result.inc = 3;
                }
            }
        } break;
        case 4: {
            if (3 < max)
            {
                U8 cont_byte[3] = {str[1], str[2], str[3]};
                if (utf8_class[cont_byte[0] >> 3] == 0 &&
                    utf8_class[cont_byte[1] >> 3] == 0 &&
                    utf8_class[cont_byte[2] >> 3] == 0)
                {
                    result.codepoint = (byte & bitmask3) << 18;
                    result.codepoint |= ((cont_byte[0] & bitmask6) << 12);
                    result.codepoint |= ((cont_byte[1] & bitmask6) <<  6);
                    result.codepoint |=  (cont_byte[2] & bitmask6);
                    result.inc = 4;
                }
            }
        }
    }
    return result;
}

function Unicode_Decode
utf16_decode(U16 *str, U64 max)
{
    Unicode_Decode result = {1, U32_MAX};
    result.codepoint = str[0];
    result.inc = 1;
    if (max > 1 && 0xD800 <= str[0] && str[0] < 0xDC00 && 0xDC00 <= str[1] && str[1] < 0xE000)
    {
        result.codepoint = ((str[0] - 0xD800) << 10) | ((str[1] - 0xDC00) + 0x10000);
        result.inc = 2;
    }
    return result;
}

function U32
utf8_encode(U8 *str, U32 codepoint)
{
    U32 inc = 0;
    if (codepoint <= 0x7F) 
    {
        str[0] = (U8)codepoint;
        inc = 1;
    }
    else if (codepoint <= 0x7FF) 
    {
        str[0] = (bitmask2 << 6) | ((codepoint >> 6) & bitmask5);
        str[1] = bit8 | (codepoint & bitmask6);
        inc = 2;
    }
    else if (codepoint <= 0xFFFF) 
    {
        str[0] = (bitmask3 << 5) | ((codepoint >> 12) & bitmask4);
        str[1] = bit8 | ((codepoint >> 6) & bitmask6);
        str[2] = bit8 | ( codepoint       & bitmask6);
        inc = 3;
    }
    else if (codepoint <= 0x10FFFF) 
    {
        str[0] = (bitmask4 << 4) | ((codepoint >> 18) & bitmask3);
        str[1] = bit8 | ((codepoint >> 12) & bitmask6);
        str[2] = bit8 | ((codepoint >>  6) & bitmask6);
        str[3] = bit8 | ( codepoint        & bitmask6);
        inc = 4;
    }
    else 
    {
        str[0] = '?';
        inc = 1;
    }
    return inc;
}

function U32
utf16_encode(U16 *str, U32 codepoint)
{
    U32 inc = 1;
    if (codepoint == U32_MAX) 
    {
        str[0] = (U16)'?';
    }
    else if (codepoint < 0x10000) 
    {
        str[0] = (U16)codepoint;
    }
    else 
    {
        U32 v = codepoint - 0x10000;
        str[0] = safe_u16_from_u32(0xD800 + (v >> 10));
        str[1] = safe_u16_from_u32(0xDC00 + (v & bitmask10));
        inc = 2;
    }
    return(inc);
}

// ---------------------------------------
// Note: Conversion.
function Utf8
to_utf8(Arena *arena, Utf16 in)
{
    Utf8 result = {};
    if (in.count)
    {
        U64 cap = in.count*3;
        U8 *str = push_array_noz(arena, U8, cap + 1);
        U16 *ptr = in.str;
        U16 *opl = ptr + in.count;
        U64 size = 0;
        Unicode_Decode consume;
        for (;ptr < opl; ptr += consume.inc)
        {
            consume = utf16_decode(ptr, opl - ptr);
            size += utf8_encode(str + size, consume.codepoint);
        }
        str[size] = 0;
        arena_pop(arena, (cap - size));
        result = utf8(str, size);
    }
    return result;
}

function Utf16
to_utf16(Arena *arena, Utf8 in)
{
    Utf16 result = {};
    if (in.count)
    {
        U64 cap = in.count*2;
        U16 *str = push_array_noz(arena, U16, cap + 1);
        U8 *ptr = in.str;
        U8 *opl = ptr + in.count;
        U64 size = 0;
        Unicode_Decode consume = {};
        for (;ptr < opl; ptr += consume.inc)
        {
            consume = utf8_decode(ptr, opl - ptr);
            size += utf16_encode(str + size, consume.codepoint);
        }
        str[size] = 0;
        arena_pop(arena, (cap - size)*2);
        result = utf16(str, size);
    }
    return result;
}
