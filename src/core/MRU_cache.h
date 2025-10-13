#ifndef MRU_CACHE_H
#define MRU_CACHE_H

#include <stdbool.h>
#include <time.h>
#include <stdint.h>
#include "list.h"

struct MRUCacheElem{
    void* data;
};

struct MRUCacheElem* MRUCacheElem_create(void* value);

struct MRUCache{
    list* time_list;
    struct HashTable* HT;
};

struct MRUCache* MRUCache_create(bool (*const compare)(void*, void*), uint32_t (*const hash1)(void*, uint32_t), void (*delete_value)(void*));
void MRUCache_delete(struct MRUCache* cache);
void MRUCache_add(struct MRUCache* cache, void* value);
void* MRUCache_Find(struct MRUCache* cache, void* value);

#endif