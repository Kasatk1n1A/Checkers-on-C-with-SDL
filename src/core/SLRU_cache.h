#ifndef SLRU_CACHE_H
#define SLRU_CACHE_H

#include <stdbool.h>
#include <stdint.h>
#include "list.h"

struct SLRUCacheElem{
    void* data;
};

struct SLRUCacheElem* SLRUCacheElem_create(void* value);

struct SLRUCache{
    list* weak_list;
    list* strong_list;
    struct HashTable* HT;
};

struct SLRUCache* SLRUCache_create(bool (*const compare)(void*, void*), uint32_t (*const hash1)(void*, uint32_t), void (*delete_value)(void*));
void SLRUCache_delete(struct SLRUCache* cache);
void SLRUCache_add(struct SLRUCache* cache, void* value);
void* SLRUCache_Find(struct SLRUCache* cache, void* value);


#endif