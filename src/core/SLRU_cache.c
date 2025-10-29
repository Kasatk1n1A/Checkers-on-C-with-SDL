#include "SLRU_cache.h"
#include "list.h"
#include "hash_table.h"
#include "Transpositions.h"
#include <stdio.h>

static uint32_t max_size_of_cache = 100;

struct SLRUCacheElem* SLRUCacheElem_create(void* value){
    struct SLRUCacheElem* elem = (struct SLRUCacheElem*)malloc(sizeof(struct SLRUCacheElem));
    elem->data = value;

    return elem;
}

void SLRUCacheElem_delete(struct SLRUCacheElem* value){
    free(value);
}

void* _SLRUCacheElem_delete_(void* value){
    free(value);
    return NULL;
}

struct SLRUCache* SLRUCache_create(bool (*const compare)(void*, void*), uint32_t (*const hash1)(void*, uint32_t), void (*delete_value)(void*)){
    struct SLRUCache* cache = (struct SLRUCache*)malloc(sizeof(struct SLRUCache));
    cache->weak_list = list_create();
    cache->strong_list = list_create();
    cache->HT = HashTable_create(compare, hash1, delete_value);

    return cache;
}

void SLRUCache_delete(struct SLRUCache* cache){
    list_destroy(cache->weak_list);
    list_destroy(cache->strong_list);
    HashTable_delete(cache->HT);
    free(cache);
}

void SLRUCache_add(struct SLRUCache* cache, void* value){
    if (!HashTable_Find(cache->HT, value)){
        /* if weak list is overflowed we should to
            remove LRU element from cache */
        if (list_size(cache->weak_list) >= max_size_of_cache / 2){
            void* elem_for_remove = list_pop_front(cache->weak_list);
            HashTable_Remove(cache->HT, elem_for_remove);
        }
          
        HashTable_Add(cache->HT, value);
        list_push_back(cache->weak_list, value);
        ((Transposition*)value)->in_strong_list = false;
    } else {
        if (!((Transposition*)value)->in_strong_list){
            /* if the element is in the weak list
               we move it in strong list */
            list_remove(cache->weak_list, list_find(cache->weak_list, value));
            list_push_back(cache->strong_list, value);
            ((Transposition*)value)->in_strong_list = true;

            /* if strong list overflowed we should to move
               LRU element in weak list */
            if (list_size(cache->strong_list) >= max_size_of_cache / 2){
                void* elem_for_move_in_weak_list = list_pop_front(cache->strong_list);
                list_push_back(cache->weak_list, elem_for_move_in_weak_list);
                ((Transposition*)elem_for_move_in_weak_list)->in_strong_list = false;
            }

            /* if weak list is overflowed we should to
               remove LRU element from cache */
            if (list_size(cache->weak_list) >= max_size_of_cache / 2){
                void* elem_for_remove = list_pop_front(cache->weak_list);
                HashTable_Remove(cache->HT, elem_for_remove);
            }
        } else {
            /* if element is in strong list we should 
               to move it in begin of list */
            list_remove(cache->strong_list, list_find(cache->strong_list, value));
            list_push_back(cache->strong_list, value);
        }
    }
}

void* SLRUCache_Find(struct SLRUCache* cache, void* value){
    Transposition* result = (Transposition*)HashTable_Find(cache->HT, value);
    if (!result){
        return NULL;
    }

    if (result->in_strong_list){
        list_remove(cache->strong_list, list_find(cache->strong_list, value));
        list_push_back(cache->strong_list, value);
    } else {
        list_remove(cache->weak_list, list_find(cache->weak_list, value));
        list_push_back(cache->strong_list, value);
        result->in_strong_list = true;
    }

    return (void*)result;
}

void* SLRUCache_remove_from_list(struct SLRUCache* cache, void* value){
    if (((Transposition*)value)->in_strong_list){
        list_remove(cache->strong_list, list_find(cache->strong_list, value));
    } else {
        list_remove(cache->weak_list, list_find(cache->weak_list, value));
    }
}   