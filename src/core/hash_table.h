#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

struct Node{
    void* value;
    bool state;
};

// typedef bool (*const compare)(void*, void*);
// typedef uint32_t (*const Hash)(void*, uint32_t);

struct HashTable{
    struct Node** arr; // соответственно в массиве будут хранится структуры Node*
    uint32_t size; // сколько элементов у нас сейчас в массиве (без учета deleted)
    uint32_t buffer_size; // размер самого массива, сколько памяти выделено под хранение нашей таблицы
    uint32_t size_all_non_nullptr; // сколько элементов у нас сейчас в массиве (с учетом deleted)
    bool (*compare)(void*, void*);
    uint32_t (*hash1)(void*, uint32_t);
    void (*delete_value)(void*);
};

struct HashTable* HashTable_create(bool (*const compare)(void*, void*), uint32_t (*const hash1)(void*, uint32_t), void (*delete_value)(void*));
void HashTable_delete(struct HashTable* HashTable);
void HashTable_resize(struct HashTable* HT);
void HashTable_HashTable_Rehash(struct HashTable* HT);
void* HashTable_Find(struct HashTable* HT, void* value);
bool HashTable_Remove(struct HashTable* HT, void* value);
bool HashTable_Add(struct HashTable* HT, void* value);
void HashTable_clean(struct HashTable* HT);

#endif