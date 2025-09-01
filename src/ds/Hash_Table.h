// Copyright (c) 2025 Seong Woo Lee. All rights reserved.
#ifndef LSW_HASH_TABLE_H
#define LSW_HASH_TABLE_H

// ------------------------------------------------------
// @Note: Entry count spec'ed to be a power of 2.
//


typedef struct Table_Entry Table_Entry;
struct Table_Entry
{
    B8 filled;
    B8 tombstone_set;
    U8 key_val[];
};

typedef struct Table_Internal Table_Internal;
struct Table_Internal
{
    Arena *arena;
    U64 entry_count;
    U64 entry_size;
    Table_Entry *entries;
};

function Table_Internal
ht_init_internal(Arena *arena, U64 entry_count, U64 entry_size)
{
    Table_Internal result = {};
    result.arena       = arena;
    result.entry_count = entry_count;
    result.entry_size  = entry_size;
    result.entries     = (Table_Entry *)arena_push(arena, entry_size*entry_count);
    return result;
}

#define TABLE_ENTRY_COUNT 1024
static_assert(is_pow2(TABLE_ENTRY_COUNT)); // @Note: Since we're using i*i as a probing function.
#define Table(arena_init, key_type, val_type) struct {\
    union {\
        Table_Internal __table__ = ht_init_internal(arena_init, TABLE_ENTRY_COUNT, sizeof(Table_Entry) + sizeof(key_type) + sizeof(val_type));\
        key_type *key_payload;\
        val_type *val_payload;\
    };\
}

function U64
hash_simple(U8 *data, U64 length)
{
    // @Todo: Better hash function.
    int result = 0;
    for (U64 i = 0; i < length; ++i)
    { result += data[i]; }
    return result;
}

function U64
ht_probing_function(U64 iter)
{
    // @Todo: Better probing function?
    return iter*iter;
}

function Table_Entry *
ht_search_empty_entry(Table_Entry *entries, U64 entry_count, U64 entry_size, U64 home_position)
{
    for (U64 i = 0; i < entry_count; ++i)
    {
        U64 probe = ht_probing_function(i); 
        U64 index = (home_position + probe) % entry_count;
        Table_Entry *entry = (Table_Entry *)((U8 *)entries + index*entry_size);
        if (!entry->filled) // empty
        {
            return entry;
        }
    } 
    return NULL;
}

#if 0
function Table_Entry *
ht_get(Table_Entry *entries, U64 entry_count, U64 entry_size, U64 home_position, U8 *key, U64 length)
{
    for (U64 i = 0; i < entry_count; ++i)
    {
        U64 probe = ht_probing_function(i); 
        U64 index = (home_position + probe) % entry_count;
        Table_Entry *entry = (Table_Entry *)((U8 *)entries + index*entry_size);
        if (!entry->tombstone_set)
        {
            if (entry->filled)
            {
                for (U64 c = 0; c < length; ++c)
                {
                    if (string8_equal(entry->key_val, key, length))
                    {
                        return entry;
                    }
                }
            }
            else
            {
                return NULL;
            }
        }
    } 

    assume(! "invalid code path.");
}
#endif

#define ht_put(t, key, val)\
{\
    sizeof(*(t)->key_payload = key);\
    sizeof(*(t)->val_payload = val);\
    U64 hashed = hash_simple((U8 *)(&((int){key})), sizeof(key)) \
    U64 home_position = hashed % ((t)->entry_count);\
    Table_Entry *entry = ht_search_empty_entry((t)->entries, (t)->entry_count, sizeof(Table_Entry)+sizeof(*(t)->key_payload)+sizeof(*(t)->val_payload), home_position);\
    assume(entry);\
    Table_Entry *header = (Table_Entry *)entry;\
    header->filled = true;\
    decltype((t)->key_payload) key_ptr = (decltype((t)->key_payload))(entry + sizeof(Table_Entry));\
    decltype((t)->val_payload) val_ptr = (decltype((t)->val_payload))((U8 *)key_ptr + sizeof(*(t)->key_payload));\
    *key_ptr = key;\
    *val_ptr = val;\
}





#endif // LSW_HASH_TABLE_H
