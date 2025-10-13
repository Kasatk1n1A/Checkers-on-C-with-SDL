#include "LFU_cache.h"
#include "list.h"
#include "hash_table.h"
#include "Transpositions.h"

#define MAX_LIFETIME 5 * 60 * 1000

static uint32_t max_size_of_cache = 1000;

struct LFUCacheElem* LFUCacheElem_create(void* value){
    struct LFUCacheElem* elem = (struct LFUCacheElem*)malloc(sizeof(struct LFUCacheElem));
    elem->data = value;
    elem->frequency = 0;

    return elem;
}

bool LFUCacheElem_smaller_or_equal(list_elem* _el1, list_elem* _el2){
    struct LFUCacheElem* el1 = (struct LFUCacheElem*)_el1->data;
    struct LFUCacheElem* el2 = (struct LFUCacheElem*)_el2->data;

    if (el1->frequency <= el2->frequency)
        return true;
    else
        return false;
}

void LFUCacheElem_delete(struct LFUCacheElem* value){
    free(value);
}

void* _LFUCacheElem_delete_(void* value){
    LFUCacheElem_delete((struct LFUCacheElem*)value);
    return NULL;
}

struct LFUCache* LFUCache_create(bool (*const compare)(void*, void*), uint32_t (*const hash1)(void*, uint32_t), void (*delete_value)(void*)){
    struct LFUCache* cache = (struct LFUCache*)malloc(sizeof(struct LFUCache));
    cache->time_list = list_create();
    cache->HT = HashTable_create(compare, hash1, delete_value);

    return cache;
}

void LFUCache_delete(struct LFUCache* cache){
    list_for_each(cache->time_list, _LFUCacheElem_delete_);
    list_destroy(cache->time_list);
    HashTable_delete(cache->HT);
    free(cache);
}

void LFUCache_add(struct LFUCache* cache, void* value){
    struct LFUCacheElem* elem = LFUCacheElem_create(value);

    if (!HashTable_Find(cache->HT, value)){
        /* If hash table max size delete oldest element and insert
            new element */
        if (cache->HT->size >= max_size_of_cache){
            HashTable_Remove(cache->HT, list_max(cache->time_list, LFUCacheElem_smaller_or_equal));
        }
        HashTable_Add(cache->HT, value);
    } else {
        list_remove(cache->time_list, list_find(cache->time_list, elem));
    }
    list_push_back(cache->time_list, elem);
    ++elem->frequency;
}

void* LFUCache_Find(struct LFUCache* cache, void* value){
    void* result = HashTable_Find(cache->HT, value);
    struct LFUCacheElem* elem = LFUCacheElem_create(value);
    if (result){
        list_remove(cache->time_list, list_find(cache->time_list, elem));
        list_push_back(cache->time_list, elem);
        ++elem->frequency;
    }
    return result;
}