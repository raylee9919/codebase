// Copyright (c) 2025 Seong Woo Lee. All rights reserved.

#ifndef SWL_MATH_H
#define SWL_MATH_H

typedef union V2 V2;
union V2
{
  struct {F32 x, y; };
  F32 e[2];
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



// @NOTE: V2
function V2 v2(F32, F32);
function V2 operator +(V2, V2);
function V2 operator -(V2, V2);
function F32 dot(V2, V2);
function V2 hadamard(V2, V2);

// @NOTE: V3
function V3 v3(F32, F32, F32);
function V3 operator +(V3, V3);
function V3 operator -(V3, V3);
function F32 dot(V3, V3);
function V3 hadamard(V3, V3);

// @NOTE: V4
function V4 v4(F32, F32, F32, F32);
function V4 operator +(V4, V4);
function V4 operator -(V4, V4);
function F32 dot(V4, V4);
function V4 hadamard(V4, V4);

// @NOTE: M4x4 
function M4x4 operator *(M4x4, M4x4);
function M4x4 identity4x4(void);
function M4x4 translate4x4(M4x4, V3);
function M4x4 scale4x4(M4x4, F32);

// @NOTE: Trigonometry
function F32 sin32(F32);
function F32 cos32(F32);
function F32 tan32(F32);


function S32 round_f32_to_s32(F32);



#endif // SWL_MATH_H
