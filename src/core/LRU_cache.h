#ifndef LRU_CACHE_H
#define LRU_CACHE_H

#include <stdbool.h>
#include <time.h>
#include <stdint.h>
#include "list.h"

struct LRUCacheElem{
    void* data;
    time_t creation_time;
};

struct LRUCacheElem* LRUCacheElem_create(void* value);

struct LRUCache{
    list* time_list;
    struct HashTable* HT;
};

struct LRUCache* LRUCache_create(bool (*const compare)(void*, void*), uint32_t (*const hash1)(void*, uint32_t), void (*delete_value)(void*));
void LRUCache_delete(struct LRUCache* cache);
void LRUCache_add(struct LRUCache* cache, void* value);
void* LRUCache_Find(struct LRUCache* cache, void* value);

#endif