#include "LRU_cache.h"
#include "list.h"
#include "hash_table.h"

#define MAX_LIFETIME 5 * 60 * 1000

static uint32_t max_size_of_cache = 1000;

struct LRUCacheElem* LRUCacheElem_create(void* value){
    struct LRUCacheElem* elem = (struct LRUCacheElem*)malloc(sizeof(struct LRUCacheElem));
    elem->data = value;

    return elem;
}

void LRUCacheElem_delete(struct LRUCacheElem* value){
    free(value);
}

void* _LRUCacheElem_delete_(void* value){
    LRUCacheElem_delete((struct LRUCacheElem*)value);
}

struct LRUCache* LRUCache_create(bool (*const compare)(void*, void*), uint32_t (*const hash1)(void*, uint32_t), void (*delete_value)(void*)){
    struct LRUCache* cache = (struct LRUCache*)malloc(sizeof(struct LRUCache));
    cache->time_list = list_create();
    cache->HT = HashTable_create(compare, hash1, delete_value);

    return cache;
}

void LRUCache_delete(struct LRUCache* cache){
    list_for_each(cache->time_list, _LRUCacheElem_delete_);
    list_destroy(cache->time_list);
    HashTable_delete(cache->HT);
    free(cache);
}

void LRUCache_add(struct LRUCache* cache, void* value){
    struct LRUCacheElem* elem = LRUCacheElem_create(value);

    if (!HashTable_Find(cache->HT, value)){
        /* If hash table max size delete oldest element and insert
            new element */
        if (cache->HT->size >= max_size_of_cache){
            HashTable_Remove(cache->HT, list_pop_front(cache->time_list));
        }
        HashTable_Add(cache->HT, value);
    } else {
        list_remove(cache->time_list, list_find(cache->time_list, elem));
    }
    list_push_back(cache->time_list, elem);
}

void* LRUCache_Find(struct LRUCache* cache, void* value){
    return HashTable_Find(cache->HT, value);
}