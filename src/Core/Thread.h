// Copyright (c) 2025 Seong Woo Lee. All rights reserved.

#ifndef LSW_THREAD_H
#define LSW_THREAD_H

struct Thread_Context
{
    Arena *scratch_arena;
};

function void thread_init(void);
function void thread_main_init(void);

thread_local Thread_Context tctx;


#endif // LSW_THREAD_H
