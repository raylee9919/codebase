// Copyright (c) 2025 Seong Woo Lee. All rights reserved.

// -----------------------
// Note: V2
function V2
v2(F32 x, F32 y)
{
    V2 result;
    result.x = x;
    result.y = y;
    return result;
}

function V2
operator + (V2 a, V2 b)
{
    V2 result = V2{a.x + b.x, a.y + b.y};
    return result;
}

function V2&
operator += (V2& a, V2 b)
{
    a.x += b.x;
    a.y += b.y;
    return a;
}

function V2
operator - (V2 a, V2 b)
{
    V2 result = V2{a.x - b.x, a.y - b.y};
    return result;
}

function V2&
operator -= (V2& a, V2 b)
{
    a.x -= b.x;
    a.y -= b.y;
    return a;
}

function V2
operator * (F32 f, V2 v)
{
    V2 result = v;
    result.x *= f;
    result.y *= f;
    return result;
}

function V2
operator * (V2 v, F32 f)
{
    V2 result = v;
    result.x *= f;
    result.y *= f;
    return result;
}

function F32
dot(V2 a, V2 b)
{
    F32 result = a.x*b.x + a.y*b.y;
    return result;
}

function V2
hadamard(V2 a, V2 b)
{
    V2 result = V2{a.x*b.x, a.y*b.y};
    return result;
}

// -----------------------
// Note: V3
function V3
v3(F32 x, F32 y, F32 z)
{
    V3 result;
    result.x = x;
    result.y = y;
    result.z = z;
    return result;
}

function V3
operator +(V3 a, V3 b)
{
    V3 result = V3{a.x + b.x, a.y + b.y, a.z + b.z};
    return result;
}

function V3
operator -(V3 a, V3 b)
{
    V3 result = V3{a.x - b.x, a.y - b.y, a.z - b.z};
    return result;
}

function F32
dot(V3 a, V3 b)
{
    F32 result = a.x*b.x + a.y*b.y + a.z*b.z;
    return result;
}

function V3
hadamard(V3 a, V3 b)
{
    V3 result = V3{a.x*b.x, a.y*b.y, a.z*b.z};
    return result;
}

// -----------------------
// Note: V4
function V4
v4(F32 r, F32 g, F32 b, F32 a)
{
    V4 result;
    result.r = r;
    result.g = g;
    result.b = b;
    result.a = a;
    return result;
}

function V4
operator +(V4 a, V4 b)
{
    V4 result = V4{a.x + b.x, a.y + b.y, a.z + b.z, a.z + b.z};
    return result;
}

function V4
operator -(V4 a, V4 b)
{
    V4 result = V4{a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w};
    return result;
}

function F32
dot(V4 a, V4 b)
{
    F32 result = a.x*b.x + a.y*b.y + a.z*b.z + a.w*b.w;
    return result;
}

function V4
hadamard(V4 a, V4 b)
{
    V4 result = V4{a.x*b.x, a.y*b.y, a.z*b.z, a.w*b.w};
    return result;
}

// -----------------------
// Note: M4x4 
function M4x4
operator *(M4x4 a, M4x4 b)
{
    M4x4 result = {};
    for (S32 r = 0; r < 4; ++r)
        for (S32 c = 0; c < 4; ++c)
            for (S32 i = 0; i < 4; ++i) 
            { result.e[r][c] = a.e[r][i] * b.e[c][i]; }
    return result;
}

function M4x4
identity4x4(void)
{
    M4x4 result =
    {{
         1, 0, 0, 0,
         0, 1, 0, 0,
         0, 0, 1, 0,
         0, 0, 0, 1
     }};
    return result;
}

function M4x4
translate4x4(M4x4 m, V3 v)
{
    M4x4 result = m;
    result.e[0][3] += v.x;
    result.e[1][3] += v.y;
    result.e[2][3] += v.z;
    return result;
}

function M4x4
scale4x4(M4x4 m, F32 f)
{
    M4x4 result = m;
    result.e[0][0] *= f;
    result.e[1][1] *= f;
    result.e[2][2] *= f;
    return result;
}

//
// Note: Trigonometry
//
function F32
sin32(F32 f)
{
    F32 result = (F32)sin(f);
    return result;
}

function F32
cos32(F32 f)
{
    F32 result = (F32)cos(f);
    return result;
}

function F32
tan32(F32 f)
{
    F32 result = (F32)tan(f);
    return result;
}

// -----------------------
// Note: Round 
function S32 
round_f32_to_s32(F32 x)
{
    S32 result = (S32)(x + 0.5f);
    return result;
}

// -----------------------
// Note: Normalize 
function F32
normalize01(V2 range, F32 val)
{
    F32 result = 0.0f;
    F32 r = (range.y - range.x);
    F32 t = (val - range.x);
    if (t != 0.0f)
    { result = clamp(r/t, 0.0f, 1.0f); }
    return result;
}

// -----------------------
// Note: Geometry 
function B32
intersects(AABB2 box, V2 point)
{
    B32 result = false;
    // @Todo: Pixel perfect.
    if (point.x >= box.min.x && point.x < box.max.x &&
        point.y >= box.min.y && point.y < box.max.y) 
    { result = true; }
    return result;
}

function B32
intersects(AABB2 a, AABB2 b)
{
    B32 result = false;
    V2 half_dim = (a.max - a.min) * 0.5f;
    a.min -= half_dim;
    a.max += half_dim;
    V2 point = (b.min + b.max) * 0.5f;
    result = intersects(a, point);
    return result;
}

function AABB2
intersection(AABB2 a, AABB2 b)
{
    AABB2 result = {};
    if (intersects(a, b))
    {
        result.min.x = (a.min.x >= b.max.x) ? (a.min.x) : (b.min.x);
        result.max.x = (a.max.x <  b.max.x) ? (a.max.x) : (b.max.x);
        result.min.y = (a.min.y >= b.max.y) ? (a.min.y) : (b.min.y);
        result.max.y = (a.max.y <  b.max.y) ? (a.max.y) : (b.max.y);
    }
    return result;
}
