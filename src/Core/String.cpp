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
function Codepoint
codepoint_from_utf8(U8 *str, U64 max)
{
    U8 utf8_class[32] =
    { 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,2,2,2,2,3,3,4,5 };

    Codepoint result = {};
    {
        result.val = ~((U32)0);
        result.advance = 1;
    }

    U8 byte = str[0];
    U8 byte_class = utf8_class[byte >> 3];

    switch (byte_class)
    {
        case 1: {
            result.val = (U32)byte;
        } break;

        case 2: {
            if (2 <= max)
            {
                U8 cont_byte = str[1];
                if (utf8_class[cont_byte >> 3] == 0)
                {
                    result.val = (byte & 0x1f) << 6;
                    result.val |= (cont_byte & 0x3f);
                    result.advance = 2;
                }
            }
        } break;

        case 3: {
            if(3 <= max)
            {
                U8 cont_byte[2] = {str[1], str[2]};
                if(utf8_class[cont_byte[0] >> 3] == 0 &&
                   utf8_class[cont_byte[1] >> 3] == 0)
                {
                    result.val = (byte & 0x1f) << 12;
                    result.val |= ((cont_byte[0] & 0x3f) << 6);
                    result.val |=  (cont_byte[1] & 0x3f);
                    result.advance = 3;
                }
            }
        } break;

        case 4: {
            if(4 <= max)
            {
                U8 cont_byte[3] = {str[1], str[2], str[3]};
                if(utf8_class[cont_byte[0] >> 3] == 0 &&
                   utf8_class[cont_byte[1] >> 3] == 0 &&
                   utf8_class[cont_byte[2] >> 3] == 0)
                {
                    result.val = (byte & 0x07) << 18;
                    result.val |= ((cont_byte[0] & 0x3f) << 12);
                    result.val |= ((cont_byte[1] & 0x3f) <<  6);
                    result.val |=  (cont_byte[2] & 0x3f);
                    result.advance = 4;
                }
            }
        } break;
    }

    return result;
}

function U32
utf16_from_codepoint(U16 *out, U32 codepoint)
{
    U32 advance = 1;
    if (codepoint == ~((U32)0))
    {
        out[0] = (U16)'?';
    }
    else if (codepoint < 0x10000)
    {
        out[0] = (U16)codepoint;
    }
    else
    {
        U64 v = codepoint - 0x10000;
        out[0] = 0xd800 + (U16)(v >> 10);
        out[1] = 0xdc00 + (U16)(v & 0x03ff);
        advance = 2;
    }
    return advance;
}

//
// Note: Conversion.
//
function Utf16        
utf16_from_utf8(Arena *arena, Utf8 str)
{
    U64 max_count = (str.count << 1);

    U16 *str16 = arena_push_array(arena, U16, max_count + 1);

    U8 *src = str.str;
    U16 *dst = str16;
    U8 *src_end = src + str.count;

    for (;;)
    {
        if (src >= src_end)
        { break; }

        Codepoint codepoint = codepoint_from_utf8(src, str.count);
        U32 advance = utf16_from_codepoint(dst, codepoint.val);

        src += codepoint.advance;
        dst += advance;
    }

    Utf16 result = {};
    {
        result.str = str16;
        result.count = dst - str16;
        result.str[result.count] = 0;
    }

    arena_pop(arena, ((max_count - result.count) << 1));

    return result;
}
