// Copyright (c) 2025 Seong Woo Lee. All rights reserved.

#ifndef SWL_CORE_H
#define SWL_CORE_H

struct Arena;
struct String8;
struct String16;

// NOTE: Third-Party Includes
#include <stdint.h>
#include <math.h>

// NOTE: Keywords
#define global          static
#define function        static
#define local_persist   static

// NOTE: Base Types
typedef int8_t   S8;
typedef int16_t  S16;
typedef int32_t  S32;
typedef int64_t  S64;
typedef uint8_t  U8;
typedef uint16_t U16;
typedef uint32_t U32;
typedef uint64_t U64;
typedef S8       B8;
typedef S16      B16;
typedef S32      B32;
typedef S64      B64;
typedef float    F32;
typedef double   F64;
typedef struct { U64 u64[2]; } U128;


// NOTE: Macro-Functions
#define assert(exp) do { if (!(exp))  __debugbreak(); } while (0)
#define array_count(arr) (sizeof(arr) / sizeof(arr[0]))
#define max(a, b) ((a) > (b) ? (a) : (b))
#define min(a, b) ((a) < (b) ? (a) : (b))
#define defer_loop(start, end)          for(int _i_ = ((start), 0); _i_ == 0; (_i_ += 1, (end)))
#define defer_loop_checked(begin, end)  for(int _i_ = 2 * !(begin); (_i_ == 2 ? ((end), 0) : !_i_); _i_ += 1, (end))
#define align_pow2(x,b)                 (((x) + (b) - 1)&(~((b) - 1)))
#define align_down_pow2(x,b)            ((x)&(~((b) - 1)))
#define clamp(x, lo, hi)  min(max(x, lo), hi)
#define clamp_high(x, hi) min(x, hi)
#define clamp_low(x, lo)  max(x, lo)

#define KB(value) (   value  * 1024ll)
#define MB(value) (KB(value) * 1024ll)
#define GB(value) (MB(value) * 1024ll)
#define TB(value) (GB(value) * 1024ll)

#define U8_MAX  (255)
#define U16_MAX (65535)
#define U32_MAX (4294967295)
#define U64_MAX (18446744073709551615)


// NOTE: Functions.
function String8 read_entire_file(Arena *arena, String16 file_path);


#endif // SWL_CORE_H
