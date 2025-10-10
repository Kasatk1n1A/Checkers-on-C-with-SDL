#include "LRU_cache.h"
#include "list.h"
#include "hash_table.h"

#define MAX_LIFETIME 5 * 60 * 1000

static void LRUCache_grow_old_list(list* lst);

struct LRUCacheElem* LRUCacheElem_create(void* value){
    struct LRUCacheElem* elem = (struct LRUCacheElem*)malloc(sizeof(struct LRUCacheElem));
    elem->data = value;
    elem->creation_time = time(NULL);

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
        HashTable_Add(cache->HT, value);
    } else {
        list_remove(cache->time_list, list_find(cache->time_list, elem));
        elem->creation_time = time(NULL);
    }
    list_push_back(cache->time_list, elem);
    LRUCache_grow_old_list(cache->time_list);
}

void LRUCache_grow_old_list(list* lst){
    list_elem* curr_elem = lst->head;
    time_t curr_time = time(NULL);

    while (curr_elem != lst->tail){
        struct LRUCacheElem* curr_value = (struct LRUCacheElem*)curr_elem->data;
        if (curr_time - curr_value->creation_time > MAX_LIFETIME){
            list_elem* tmp = curr_elem;
            size_t index = list_find(lst, tmp->data);
            curr_elem = curr_elem->next;
            _LRUCacheElem_delete_(tmp->data);
            list_remove(lst, index);
            continue;
        }
        curr_elem = curr_elem->next;
    }
}

void* LRUCache_Find(struct LRUCache* cache, void* value){
    return HashTable_Find(cache->HT, value);
}