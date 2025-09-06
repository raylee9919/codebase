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

#define quick_sort(base, type, count, compare_function) qsort((base), (count), sizeof(type), (int(*)(const void *, const void *))(compare_function))

// ----------------------------------
// @Note: Memory Operations
#define memory_copy(dst, src, size) memcpy((dst), (src), (size))
#define memory_move(dst, src, size) memmove((dst), (src), (size))
#define memory_set(dst, byte, size) memset((dst), (byte), (size))
#define zero_memory(ptr, size) memory_set((ptr), 0, (size))
#define zero_array(ptr, type, count) memory_set((ptr), 0, sizeof(type)*(count))


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
// @Note: Dynamic Array
#define DYNAMIC_ARRAY_DATA(TYPE)\
    struct {\
        Arena *arena;\
        TYPE  *base;\
        U64   count_cur;\
        U64   count_max;\
    }

#define Dynamic_Array(TYPE)\
    union {\
        DYNAMIC_ARRAY_DATA(TYPE);\
        TYPE *payload;\
    }

#define DAR_RESERVE_INIT 64

#define dar_init(A, ARENA)\
    (A)->arena = ARENA;\
    (A)->base = (decltype((A)->payload))push_size(ARENA, sizeof(decltype(*(A)->payload)) * DAR_RESERVE_INIT);\
    (A)->count_cur = 0;\
    (A)->count_max = DAR_RESERVE_INIT

// @Todo: Fragmentation.
#define dar_push(A, ITEM)\
    if (((A)->count_cur >= (A)->count_max)) {\
        void *new_base = push_size((A)->arena, sizeof(decltype(*(A)->payload)) * ((A)->count_max << 1));\
        memory_copy( new_base, (A)->base, sizeof(decltype(*(A)->payload)) * (A)->count_max );\
        (A)->count_max <<= 1;\
    }\
    *(decltype((A)->payload))( ((decltype((A)->payload))((A)->base)) + ((A)->count_cur++) ) = ITEM;

// Sets the length of an array to at least N.
#define dar_reserve(A, N)\
    if ((A)->count_max < N) {\
        void *new_base = push_size( (A)->arena, sizeof(decltype(*(A)->payload)) * N );\
        memory_copy( new_base, (A)->base, sizeof(decltype(*(A)->payload)) * (A)->count_cur );\
        (A)->count_max = N; \
    }


// ---------------------------------------
// @Note: Constants

global U64 U64_MAX = 0xffffffffffffffffull;
global U32 U32_MAX = 0xffffffff;
global U16 U16_MAX = 0xffff;
global U8  U8_MAX  = 0xff;

global S64 S64_MAX = (S64)0x7fffffffffffffffll;
global S32 S32_MAX = (S32)0x7fffffff;
global S16 S16_MAX = (S16)0x7fff;
global S8  S8_MAX  = (S8)0x7f;

global const U32 bitmask1  = 0x00000001;
global const U32 bitmask2  = 0x00000003;
global const U32 bitmask3  = 0x00000007;
global const U32 bitmask4  = 0x0000000f;
global const U32 bitmask5  = 0x0000001f;
global const U32 bitmask6  = 0x0000003f;
global const U32 bitmask7  = 0x0000007f;
global const U32 bitmask8  = 0x000000ff;
global const U32 bitmask9  = 0x000001ff;
global const U32 bitmask10 = 0x000003ff;
global const U32 bitmask11 = 0x000007ff;
global const U32 bitmask12 = 0x00000fff;
global const U32 bitmask13 = 0x00001fff;
global const U32 bitmask14 = 0x00003fff;
global const U32 bitmask15 = 0x00007fff;
global const U32 bitmask16 = 0x0000ffff;
global const U32 bitmask17 = 0x0001ffff;
global const U32 bitmask18 = 0x0003ffff;
global const U32 bitmask19 = 0x0007ffff;
global const U32 bitmask20 = 0x000fffff;
global const U32 bitmask21 = 0x001fffff;
global const U32 bitmask22 = 0x003fffff;
global const U32 bitmask23 = 0x007fffff;
global const U32 bitmask24 = 0x00ffffff;
global const U32 bitmask25 = 0x01ffffff;
global const U32 bitmask26 = 0x03ffffff;
global const U32 bitmask27 = 0x07ffffff;
global const U32 bitmask28 = 0x0fffffff;
global const U32 bitmask29 = 0x1fffffff;
global const U32 bitmask30 = 0x3fffffff;
global const U32 bitmask31 = 0x7fffffff;
global const U32 bitmask32 = 0xffffffff;

global const U64 bitmask33 = 0x00000001ffffffffull;
global const U64 bitmask34 = 0x00000003ffffffffull;
global const U64 bitmask35 = 0x00000007ffffffffull;
global const U64 bitmask36 = 0x0000000fffffffffull;
global const U64 bitmask37 = 0x0000001fffffffffull;
global const U64 bitmask38 = 0x0000003fffffffffull;
global const U64 bitmask39 = 0x0000007fffffffffull;
global const U64 bitmask40 = 0x000000ffffffffffull;
global const U64 bitmask41 = 0x000001ffffffffffull;
global const U64 bitmask42 = 0x000003ffffffffffull;
global const U64 bitmask43 = 0x000007ffffffffffull;
global const U64 bitmask44 = 0x00000fffffffffffull;
global const U64 bitmask45 = 0x00001fffffffffffull;
global const U64 bitmask46 = 0x00003fffffffffffull;
global const U64 bitmask47 = 0x00007fffffffffffull;
global const U64 bitmask48 = 0x0000ffffffffffffull;
global const U64 bitmask49 = 0x0001ffffffffffffull;
global const U64 bitmask50 = 0x0003ffffffffffffull;
global const U64 bitmask51 = 0x0007ffffffffffffull;
global const U64 bitmask52 = 0x000fffffffffffffull;
global const U64 bitmask53 = 0x001fffffffffffffull;
global const U64 bitmask54 = 0x003fffffffffffffull;
global const U64 bitmask55 = 0x007fffffffffffffull;
global const U64 bitmask56 = 0x00ffffffffffffffull;
global const U64 bitmask57 = 0x01ffffffffffffffull;
global const U64 bitmask58 = 0x03ffffffffffffffull;
global const U64 bitmask59 = 0x07ffffffffffffffull;
global const U64 bitmask60 = 0x0fffffffffffffffull;
global const U64 bitmask61 = 0x1fffffffffffffffull;
global const U64 bitmask62 = 0x3fffffffffffffffull;
global const U64 bitmask63 = 0x7fffffffffffffffull;
global const U64 bitmask64 = 0xffffffffffffffffull;

global const U32 bit1  = (1<<0);
global const U32 bit2  = (1<<1);
global const U32 bit3  = (1<<2);
global const U32 bit4  = (1<<3);
global const U32 bit5  = (1<<4);
global const U32 bit6  = (1<<5);
global const U32 bit7  = (1<<6);
global const U32 bit8  = (1<<7);
global const U32 bit9  = (1<<8);
global const U32 bit10 = (1<<9);
global const U32 bit11 = (1<<10);
global const U32 bit12 = (1<<11);
global const U32 bit13 = (1<<12);
global const U32 bit14 = (1<<13);
global const U32 bit15 = (1<<14);
global const U32 bit16 = (1<<15);
global const U32 bit17 = (1<<16);
global const U32 bit18 = (1<<17);
global const U32 bit19 = (1<<18);
global const U32 bit20 = (1<<19);
global const U32 bit21 = (1<<20);
global const U32 bit22 = (1<<21);
global const U32 bit23 = (1<<22);
global const U32 bit24 = (1<<23);
global const U32 bit25 = (1<<24);
global const U32 bit26 = (1<<25);
global const U32 bit27 = (1<<26);
global const U32 bit28 = (1<<27);
global const U32 bit29 = (1<<28);
global const U32 bit30 = (1<<29);
global const U32 bit31 = (1<<30);
global const U32 bit32 = (U32)(1<<31);

global const U64 bit33 = (1ull<<32);
global const U64 bit34 = (1ull<<33);
global const U64 bit35 = (1ull<<34);
global const U64 bit36 = (1ull<<35);
global const U64 bit37 = (1ull<<36);
global const U64 bit38 = (1ull<<37);
global const U64 bit39 = (1ull<<38);
global const U64 bit40 = (1ull<<39);
global const U64 bit41 = (1ull<<40);
global const U64 bit42 = (1ull<<41);
global const U64 bit43 = (1ull<<42);
global const U64 bit44 = (1ull<<43);
global const U64 bit45 = (1ull<<44);
global const U64 bit46 = (1ull<<45);
global const U64 bit47 = (1ull<<46);
global const U64 bit48 = (1ull<<47);
global const U64 bit49 = (1ull<<48);
global const U64 bit50 = (1ull<<49);
global const U64 bit51 = (1ull<<50);
global const U64 bit52 = (1ull<<51);
global const U64 bit53 = (1ull<<52);
global const U64 bit54 = (1ull<<53);
global const U64 bit55 = (1ull<<54);
global const U64 bit56 = (1ull<<55);
global const U64 bit57 = (1ull<<56);
global const U64 bit58 = (1ull<<57);
global const U64 bit59 = (1ull<<58);
global const U64 bit60 = (1ull<<59);
global const U64 bit61 = (1ull<<60);
global const U64 bit62 = (1ull<<61);
global const U64 bit63 = (1ull<<62);
global const U64 bit64 = (1ull<<63);
    

function U16 safe_u16_from_u32(U32 x);
function U32 safe_u32_from_u64(U64 x);
function S32 safe_s32_from_s64(S64 x);
function Utf8 read_entire_file(Arena *arena, Utf8 file_path);


#endif // LSW_CORE_H
