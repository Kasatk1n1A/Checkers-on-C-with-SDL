#ifndef LFU_CACHE_H
#define LFU_CACHE_H

#include <stdbool.h>
#include <time.h>
#include <stdint.h>
#include "list.h"

struct LFUCacheElem{
    void* data;
    uint16_t frequency;
};

struct LFUCacheElem* LFUCacheElem_create(void* value);

struct LFUCache{
    list* time_list;
    struct HashTable* HT;
};

struct LFUCache* LFUCache_create(bool (*const compare)(void*, void*), uint32_t (*const hash1)(void*, uint32_t), void (*delete_value)(void*));
void LFUCache_delete(struct LFUCache* cache);
void LFUCache_add(struct LFUCache* cache, void* value);
void* LFUCache_Find(struct LFUCache* cache, void* value);

#endif