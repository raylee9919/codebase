// Copyright (c) 2025 Seong Woo Lee. All rights reserved.
#ifndef LSW_LIST_H
#define LSW_LIST_H

typedef struct List_Node List_Node;
struct List_Node
{
    List_Node *next;
    List_Node *prev;
    U8 data[]; // @Todo: Alignemnt.
};

#define List(arena_init, type) struct {                                             \
    Arena *arena = arena_init;                                                      \
    union {                                                                         \
        List_Node *sentinel = list_sentinel_init(arena_init, sizeof(List_Node));    \
        type *payload;                                                              \
    };                                                                              \
}

// @Note: Append
#define list_alloc_back(l) \
    ((decltype((l)->payload))_list_alloc_back((l)->arena, (l)->sentinel, sizeof(*(l)->payload)))

#define list_append(list, item) \
    *list_alloc_back(list) = item

// @Note: Prepend
#define list_alloc_front(l) \
    ((decltype((l)->payload))_list_alloc_front((l)->arena, (l)->sentinel, sizeof(*(l)->payload)))

#define list_prepend(list, item) \
    *list_alloc_front(list) = item

// @Note: Iterate
#define list_first_data(l) ( ( decltype((l)->payload) )((l)->sentinel->next != (l)->sentinel ? (l)->sentinel->next->data : NULL) ) 
#define list_for(it, l) for ( decltype((l)->payload) it = list_first_data(l); \
                              it != NULL; \
                              it = (decltype((l)->payload))list_next_data((l)->sentinel, it) )

#endif // LSW_LIST_H
