// Copyright (c) 2025 Seong Woo Lee. All rights reserved.

function void
thread_init(void)
{
    tctx.scratch_arena = arena_alloc(kilobytes(256));
}

function void
thread_main_init(void)
{
    tctx.scratch_arena = arena_alloc(kilobytes(256));
};
