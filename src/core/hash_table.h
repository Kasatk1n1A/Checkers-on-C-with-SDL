#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

struct Node{
    void* value;
    bool state;
};

typedef bool (*Compare)(void*, void*);
typedef uint32_t (*Hash)(void*, uint32_t);

struct HashTable{
    struct Node** arr; // соответственно в массиве будут хранится структуры Node*
    uint32_t size; // сколько элементов у нас сейчас в массиве (без учета deleted)
    uint32_t buffer_size; // размер самого массива, сколько памяти выделено под хранение нашей таблицы
    uint32_t size_all_non_nullptr; // сколько элементов у нас сейчас в массиве (с учетом deleted)
    Compare compare;
    Hash hash1;
};

struct HashTable* HashTable_create(Compare compare, Hash hash1);
void HashTable_delete(struct HashTable* HashTable);
void HashTable_resize(struct HashTable* HT);
void HashTable_HashTable_Rehash(struct HashTable* HT);
void* HashTable_Find(struct HashTable* HT, void* value);
bool HashTable_Remove(struct HashTable* HT, void* value);
bool HashTable_Add(struct HashTable* HT, void* value);

#endif