// Copyright (c) 2025 Seong Woo Lee. All rights reserved.

// STUDY: AFAIK, Windows pre-commits some pages ahead to avoid heavy page-faults.
// Is there a point of segregating reserving and committing?


/*
 * NOTE: Simply returns base as NULL if fails.
 *        Better control for users:
 *
 *          Arena arena = alloc_arena(1024);
 *          if arena.base == NULL:
 *             save_game_and_abort();
 *
 * TODO: Is it too cubersome? Should callback intervene whenever an alloc fails?
 */

function U64
get_page_aligned_size(U64 size)
{
    U64 page_size = os_get_page_size();
    U64 result= align_pow2(size, page_size);
    return result;
}

function Arena *
arena_alloc(U64 size = ARENA_DEFAULT_RESERVE_SIZE)
{
    U64 arena_plus_requested_size = ARENA_HEADER_ALIGNED_SIZE + size;
    U64 actual_size = get_page_aligned_size(arena_plus_requested_size);

    B32 commit_now = true;

    void *ptr = os_reserve(actual_size, commit_now); // IMPORTANT: spec'ed to pass page-aligned size.
    Arena *result = (Arena *)ptr;
    {
        result->base     = (U8 *)ptr + ARENA_HEADER_ALIGNED_SIZE;
        result->size     = actual_size - ARENA_HEADER_ALIGNED_SIZE;
        result->used     = 0;
        result->next     = NULL;
        result->prev     = NULL;
        result->current  = result;
    }

    return result;
}

function void
arena_dealloc(Arena *arena)
{
    Arena *node = arena->current;
    while (node != NULL)
    {
        void *ptr = node;
        node = node->prev;
        os_release(ptr);
    }
}

function void
arena_pop(Arena *arena, U64 size)
{
    Arena *node = arena->current;
    for (;;)
    {
        if (node == NULL)
        { break; }

        if (node->used >= size)
        {
            node->used -= size;
            arena->current = node;
            break;
        }
        else
        {
            node->used = 0;
            arena->current = node->prev;
            size -= node->used;
            node = node->prev;
        }
    }
}

function void
arena_clear(Arena *arena)
{
    for (Arena *node = arena->current; node != NULL; node = node->prev)
    {
        node->used = 0; 
        arena->current = node;
    }
}

function void *
arena_push(Arena *arena, U64 size)
{
    void *result = NULL;

    U64 actual_size = get_page_aligned_size(size);

    Arena *current = arena->current;

    if (current->used + actual_size <= current->size)
    {
        result = current->base + current->used;
        current->used += actual_size;
    }
    else
    {
        Arena *new_arena = arena_alloc(actual_size);
        new_arena->prev = current;
        arena->current = new_arena;

        result = new_arena->base;
        new_arena->used += actual_size;
    }

    return result;
}

function void *
_arena_bootstrap(U64 struct_size, U64 arena_offset)
{
    Arena *arena = arena_alloc();
    U8 *result = (U8 *)arena_push(arena, struct_size);
    Arena **actual_arena = ((Arena **)(result + arena_offset));
    *actual_arena = arena;
    return result;
}

function Temporary_Arena
scratch_begin(void)
{
    Temporary_Arena result = {};
    {
        result.arena  = tctx.scratch_arena;
        result.used   = tctx.scratch_arena->used;
    }
    return result;
}

function void
scratch_end(Temporary_Arena tmp)
{
    arena_pop(tmp.arena, tmp.arena->used - tmp.used);
}
