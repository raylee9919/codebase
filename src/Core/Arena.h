// Copyright (c) 2025 Seong Woo Lee. All rights reserved.
#ifndef LSW_ARENA_H
#define LSW_ARENA_H

#define ARENA_DEFAULT_RESERVE_SIZE  megabytes(64)
// #define ARENA_DEFAULT_COMMIT_SIZE   KB(64)
#define ARENA_HEADER_ALIGNED_SIZE 48

typedef struct Arena Arena;
struct Arena
{
    U8 *base;
    U64 size;
    U64 used;

    Arena *next;
    Arena *prev;

    Arena *current;
};
static_assert(sizeof(Arena) <= ARENA_HEADER_ALIGNED_SIZE);

typedef struct Temporary_Arena Temporary_Arena;
struct Temporary_Arena
{
    Arena *arena;
    U64 used;
};



// Note: Declaration.
function Arena *arena_alloc(U64 size);
function void arena_dealloc(Arena *arena);
function void arena_pop(Arena *arena, U64 size);
function void arena_clear(Arena *arena);
function void *arena_push(Arena *arena, U64 size);
#define arena_push_struct(arena, type) ((type *)arena_push(arena, sizeof(type)))
#define arena_push_array(arena, type, count) ((type *)arena_push(arena, sizeof(type)*count))

function Temporary_Arena scratch_begin(void);
function void scratch_end(Temporary_Arena tmp);

#endif // LSW_ARENA_H
