// Copyright (c) 2025 Seong Woo Lee. All rights reserved.

#ifndef LSW_CORE_H
#define LSW_CORE_H

struct Arena;
struct Utf8;
struct Utf16;

// @Note: Third-Party Includes
#include <stdint.h>
#include <math.h>

// @Note: Keywords
#define global          static
#define function        static
#define local_persist   static

// @Note: Base Types
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


// @Note: Macro-Functions
#define assert(exp) do { if (!(exp)) __debugbreak(); } while (0)
#define assume(exp) do { if (!(exp)) __debugbreak(); } while (0)
#define array_count(arr) (sizeof(arr) / sizeof(arr[0]))
#define max(a, b) ((a) > (b) ? (a) : (b))
#define min(a, b) ((a) < (b) ? (a) : (b))
#define u64_from_ptr(p) (U64)(((U8 *)p) - 0)
#define offset_of(type, member) u64_from_ptr(&((type *)0)->member)
#define base_from_member(type, member_name, ptr) (type *)((U8 *)(ptr) - offset_of(type, member_name))
#define defer_loop(start, end)          for(int _i_ = ((start), 0); _i_ == 0; (_i_ += 1, (end)))
#define defer_loop_checked(begin, end)  for(int _i_ = 2 * !(begin); (_i_ == 2 ? ((end), 0) : !_i_); _i_ += 1, (end))
#define align_pow2(x,b)                 (((x) + (b) - 1)&(~((b) - 1)))
#define align_down_pow2(x,b)            ((x)&(~((b) - 1)))
#define clamp(x, lo, hi)  min(max(x, lo), hi)
#define clamp_high(x, hi) min(x, hi)
#define clamp_low(x, lo)  max(x, lo)
#define is_pow2(x) (((x) & ((x)-1)) == 0)

#define kilobytes(x) (x * 1024ll)
#define megabytes(x) (kilobytes(x) * 1024ll)
#define gigabytes(x) (megabytes(x) * 1024ll)
#define terabytes(x) (gigabytes(x) * 1024ll)

#define U8_MAX  (255)
#define U16_MAX (65535)
#define U32_MAX (4294967295)
#define U64_MAX (18446744073709551615)

#define quick_sort(base, type, count, compare_function) qsort((base), (count), sizeof(type), (int(*)(const void *, const void *))(compare_function))

// ----------------------------------
// @Note: Memory Operations
#define memory_copy(dst, src, size) memcpy((dst), (src), (size))
#define memory_move(dst, src, size) memmove((dst), (src), (size))
#define memory_set(dst, byte, size) memset((dst), (byte), (size))
#define memory_zero(ptr, size) memory_set((ptr), 0, (size))

// ----------------------------------
// @Note: Doubly-Linked List.
#define dll_append(sentinel, node) \
    assume(sentinel); \
    (node)->next = (sentinel); \
    (node)->prev = (sentinel)->prev; \
    (sentinel)->prev->next = (node); \
    (sentinel)->prev = (node);

#define dll_for(sentinel, it) \
    assume(sentinel); \
    for (decltype(sentinel) it = sentinel->next; it != sentinel; it = it->next)

// ----------------------------------
// @Note: Array
typedef struct 
{
    Arena *arena;
    void *base;
    U64 count_cur;
    U64 count_max;
} Dynamic_Array_Data;

function Dynamic_Array_Data
dynamic_array_data_init(Arena *arena, U64 item_size, U64 count);

#define Dynamic_Array(TYPE)\
    union {\
        Dynamic_Array_Data data;\
        TYPE *payload;\
    }

#define darr_init(A, ARENA) (A)->data = dynamic_array_data_init(ARENA, sizeof(decltype(*(A)->payload)), 64)

// @Todo: Fragmentation.
#define darr_push(A, ITEM)\
    if (((A)->data.count_cur >= (A)->data.count_max)) {\
        void *new_base = arena_push((A)->data.arena, sizeof(decltype(*(A)->payload)) * ((A)->data.count_max << 1));\
        memory_copy( new_base, (A)->data.base, sizeof(decltype(*(A)->payload)) * (A)->data.count_max );\
        (A)->data.count_max <<= 1;\
    }\
    *(decltype((A)->payload))( ((decltype((A)->payload))((A)->data.base)) + ((A)->data.count_cur++) ) = ITEM;

    



// @Note: Functions.
function Utf8 read_entire_file(Arena *arena, Utf8 file_path);


#endif // LSW_CORE_H
