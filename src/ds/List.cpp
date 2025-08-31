// Copyright (c) 2025 Seong Woo Lee. All rights reserved.

function List_Node *
list_sentinel_init(Arena *arena, U64 size)
{
    List_Node *result = (List_Node *)arena_push(arena, size);
    result->next = result;
    result->prev = result;
    return result;
}

function void *
_list_alloc_back(Arena *arena, List_Node *sentinel, U64 data_size)
{
    List_Node *node = (List_Node *)arena_push(arena, sizeof(*node) + data_size);
    assume(node);

    node->prev = sentinel->prev;
    node->next = sentinel;
    sentinel->prev->next = node;
    sentinel->prev = node;

    return node->data;
}

function void *
_list_alloc_front(Arena *arena, List_Node *sentinel, U64 data_size)
{
    List_Node *node = (List_Node *)arena_push(arena, sizeof(*node) + data_size);
    assume(node);

    node->prev = sentinel;
    node->next = sentinel->next;
    sentinel->next->prev = node;
    sentinel->next = node;

    return node->data;
}

function void *
list_next_data(List_Node *sentinel, void *data)
{
    U64 offset = offsetof(List_Node, data);
    List_Node *node = (List_Node *)((U8 *)data - offset);
    node = node->next;
    void *result = node != sentinel ? node->data : NULL;
    return result;
}
