#include "LRU_cache.h"
#include "list.h"
#include "hash_table.h"
#include <stdio.h>

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
    free(value);
    return NULL;
}

struct LRUCache* LRUCache_create(bool (*const compare)(void*, void*), uint32_t (*const hash1)(void*, uint32_t), void (*delete_value)(void*)){
    struct LRUCache* cache = (struct LRUCache*)malloc(sizeof(struct LRUCache));
    cache->time_list = list_create();
    cache->HT = HashTable_create(compare, hash1, delete_value);

    return cache;
}

void LRUCache_delete(struct LRUCache* cache){
    // list_for_each(cache->time_list, _LRUCacheElem_delete_);
    list_destroy(cache->time_list);
    HashTable_delete(cache->HT);
    free(cache);
}

void LRUCache_add(struct LRUCache* cache, void* value){
    if (!HashTable_Find(cache->HT, value)){
        /* If hash table max size delete oldest element and insert
            new element */
        if (cache->HT->size >= max_size_of_cache){
            void* elem_for_remove = list_pop_front(cache->time_list);
            HashTable_Remove(cache->HT, elem_for_remove);
            // printf("NIGGER\n");
        }
        HashTable_Add(cache->HT, value);
    } else {
        list_remove(cache->time_list, list_find(cache->time_list, value));
    }
    list_push_back(cache->time_list, value);
}

void* LRUCache_Find(struct LRUCache* cache, void* value){
    void* result = HashTable_Find(cache->HT, value);
    if (result){
        list_remove(cache->time_list, list_find(cache->time_list, value));
        list_push_back(cache->time_list, value);
    }
    return result;
}