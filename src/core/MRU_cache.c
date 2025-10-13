#include "MRU_cache.h"
#include "list.h"
#include "hash_table.h"

#define MAX_LIFETIME 5 * 60 * 1000

static uint32_t max_size_of_cache = 1000;

struct MRUCacheElem* MRUCacheElem_create(void* value){
    struct MRUCacheElem* elem = (struct MRUCacheElem*)malloc(sizeof(struct MRUCacheElem));
    elem->data = value;

    return elem;
}

void MRUCacheElem_delete(struct MRUCacheElem* value){
    free(value);
}

void* _MRUCacheElem_delete_(void* value){
    MRUCacheElem_delete((struct MRUCacheElem*)value);
}

struct MRUCache* MRUCache_create(bool (*const compare)(void*, void*), uint32_t (*const hash1)(void*, uint32_t), void (*delete_value)(void*)){
    struct MRUCache* cache = (struct MRUCache*)malloc(sizeof(struct MRUCache));
    cache->time_list = list_create();
    cache->HT = HashTable_create(compare, hash1, delete_value);

    return cache;
}

void MRUCache_delete(struct MRUCache* cache){
    list_for_each(cache->time_list, _MRUCacheElem_delete_);
    list_destroy(cache->time_list);
    HashTable_delete(cache->HT);
    free(cache);
}

void MRUCache_add(struct MRUCache* cache, void* value){
    struct MRUCacheElem* elem = MRUCacheElem_create(value);

    if (!HashTable_Find(cache->HT, value)){
        /* If hash table max size delete oldest element and insert
            new element */
        if (cache->HT->size >= max_size_of_cache){
            HashTable_Remove(cache->HT, list_pop_back(cache->time_list));
        }
        HashTable_Add(cache->HT, value);
    } else {
        list_remove(cache->time_list, list_find(cache->time_list, elem));
    }
    list_push_back(cache->time_list, elem);
}

void* MRUCache_Find(struct MRUCache* cache, void* value){
    return HashTable_Find(cache->HT, value);
}