#ifndef LRU_CACHE_H
#define LRU_CACHE_H

struct LRU_cache{
    struct list* time_list;
    struct HashTable* HT;
};




#endif