// Copyright (c) 2025 Seong Woo Lee. All rights reserved.

#ifndef LSW_MATH_H
#define LSW_MATH_H

typedef union V2 V2;
union V2
{
    struct {F32 x, y; };
    F32 e[2];
};

typedef union V2U V2U;
union V2U
{
    struct {U32 x, y; };
    U32 e[2];
};

typedef union V3 V3;
union V3
{
    struct { F32 x, y, z; };
    struct { F32 r, g, b; };
    F32 e[3];
};

typedef union V4 V4;
union V4
{
    struct { F32 r, g, b, a; };
    struct { F32 x, y, z, w; };
    F32 e[4];
};

typedef struct M4x4 M4x4;
struct M4x4
{
    F32 e[4][4];
};

typedef struct AABB2 AABB2;
struct AABB2
{
    V2 min;
    V2 max;
};


// @Note: V2
function V2 v2(F32, F32);
function V2 operator +(V2, V2);
function V2 operator -(V2, V2);
function V2 operator *(F32 f, V2 v);
function V2 operator *(V2 v, F32 f);
function F32 dot(V2, V2);
function V2 hadamard(V2, V2);

// @Note: V3
function V3 v3(F32, F32, F32);
function V3 operator +(V3, V3);
function V3 operator -(V3, V3);
function F32 dot(V3, V3);
function V3 hadamard(V3, V3);

// @Note: V4
function V4 v4(F32, F32, F32, F32);
function V4 operator +(V4, V4);
function V4 operator -(V4, V4);
function F32 dot(V4, V4);
function V4 hadamard(V4, V4);

// @Note: M4x4 
function M4x4 operator *(M4x4, M4x4);
function M4x4 identity4x4(void);
function M4x4 translate4x4(M4x4, V3);
function M4x4 scale4x4(M4x4, F32);

// @Note: Trigonometry
function F32 sin32(F32);
function F32 cos32(F32);
function F32 tan32(F32);

function S32 round_f32_to_s32(F32);

// @Note: Geometry
function B32 intersects(AABB2 a, AABB2 b);
function AABB2 intersection(AABB2 a, AABB2 b);



#endif // LSW_MATH_H
