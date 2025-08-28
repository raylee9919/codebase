// Copyright (c) 2025 Seong Woo Lee. All rights reserved.

#ifndef SWL_THREAD_H
#define SWL_THREAD_H

struct Thread_Context
{
  Arena *scratch_arena;
};

function void thread_init(void);

thread_local Thread_Context tctx;


#endif // SWL_THREAD_H
