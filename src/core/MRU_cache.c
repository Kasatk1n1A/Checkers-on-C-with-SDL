#include "MRU_cache.h"
#include "list.h"
#include "hash_table.h"

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
    free(value);
    return NULL;
}

struct MRUCache* MRUCache_create(bool (*const compare)(void*, void*), uint32_t (*const hash1)(void*, uint32_t), void (*delete_value)(void*)){
    struct MRUCache* cache = (struct MRUCache*)malloc(sizeof(struct MRUCache));
    cache->time_list = list_create();
    cache->HT = HashTable_create(compare, hash1, delete_value);

    return cache;
}

void MRUCache_delete(struct MRUCache* cache){
    list_destroy(cache->time_list);
    HashTable_delete(cache->HT);
    free(cache);
}

void MRUCache_add(struct MRUCache* cache, void* value){
    if (!HashTable_Find(cache->HT, value)){
        /* If hash table max size delete oldest element and insert
            new element */
        if (cache->HT->size >= max_size_of_cache){
            void* elem_for_remove = list_pop_front(cache->time_list);
            HashTable_Remove(cache->HT, elem_for_remove);
        }
        HashTable_Add(cache->HT, value);
    } else {
        list_remove(cache->time_list, list_find(cache->time_list, value));
    }
    list_push_front(cache->time_list, value);
}

void* MRUCache_Find(struct MRUCache* cache, void* value){
    void* result = HashTable_Find(cache->HT, value);
    if (result){
        list_remove(cache->time_list, list_find(cache->time_list, value));
        list_push_back(cache->time_list, value);
    }
    return result;
}