#ifndef HASH_TABLE_H
#define HASH_TABLE_H

struct Node{
    void* value;
    bool state;
};

struct HashTable{
    struct Node** arr; // соответственно в массиве будут хранится структуры Node*
    int size; // сколько элементов у нас сейчас в массиве (без учета deleted)
    int buffer_size; // размер самого массива, сколько памяти выделено под хранение нашей таблицы
    int size_all_non_nullptr; // сколько элементов у нас сейчас в массиве (с учетом deleted)
};

struct HashTable* HashTable_create(void);
void HashTable_delete(struct HashTable* HashTable);
void HashTable_resize(struct HashTable* HT);
void Rehash(struct HashTable* HT);
bool Find(struct HashTable* HT, void* value, int (*hash1)(void*, int), int (*hash2)(void*, int), bool (*compare)(void*, void*));
bool Remove(struct HashTable* HT, void* value, int (*hash1)(void*, int), int (*hash2)(void*, int), bool (*compare)(void*, void*));
bool Add(struct HashTable* HT, void* value, int (*hash1)(void*, int), int (*hash2)(void*, int), bool (*compare)(void*, void*));







#endif