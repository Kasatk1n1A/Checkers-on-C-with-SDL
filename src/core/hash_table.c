#include "hash_table.h"


static void swap(struct Node*** el1, struct Node*** el2);
static uint32_t rehash(uint32_t hash, uint32_t size);

static struct Node* Node_create(void* value);
static void Node_delete(struct Node* Node);

struct Node* Node_create(void* value){
    struct Node* Node = (struct Node*)malloc(sizeof(struct Node));
    Node->value = value;
    Node->state = true;
    return Node;
}

void Node_delete(struct Node* Node){
    free(Node);
}

static const int default_size = 8;
static const double rehash_size = 0.75;
static uint32_t MAX_SIZE_CACHE = 10000;

struct HashTable* HashTable_create(Compare compare, Hash hash1){
    struct HashTable* HashTable = (struct HashTable*)malloc(sizeof(struct HashTable));
    HashTable->buffer_size = (uint32_t)default_size;
    HashTable->size = 0;
    HashTable->size_all_non_nullptr = 0;
    HashTable->arr = (struct Node**)malloc(HashTable->buffer_size * sizeof(struct Node*));
    HashTable->compare = compare;
    HashTable->hash1 = hash1;
    for (uint32_t i = 0; i < HashTable->buffer_size; ++i)
        HashTable->arr[i] = NULL; // заполняем nullptr - то есть если значение отсутствует, и никто раньше по этому адресу не обращался
    
    return HashTable;
}

void HashTable_delete(struct HashTable* HashTable){
    for (uint32_t i = 0; i < HashTable->buffer_size; ++i)
        if (HashTable->arr[i])
            Node_delete(HashTable->arr[i]);
    free(HashTable->arr);
    free(HashTable);
}

void HashTable_resize(struct HashTable* HT){
    uint32_t past_buffer_size = HT->buffer_size;
    HT->buffer_size *= 2;
    HT->size_all_non_nullptr = 0;
    HT->size = 0;
    struct Node** arr2 = (struct Node**)malloc(HT->buffer_size * sizeof(struct Node*));
    for (uint32_t i = 0; i < HT->buffer_size; ++i)
        arr2[i] = NULL;
    swap(&HT->arr, &arr2);
    for (uint32_t i = 0; i < past_buffer_size; ++i)
    {
        if (arr2[i] && arr2[i]->state)
            HashTable_Add(HT, arr2[i]->value); // добавляем элементы в новый массив
    }
    // удаление предыдущего массива
    for (uint32_t i = 0; i < past_buffer_size; ++i)
        if (arr2[i])
            free(arr2[i]);
    free(arr2);
}

void HashTable_Rehash(struct HashTable* HT)
{
    HT->size_all_non_nullptr = 0;
    HT->size = 0;
    struct Node** arr2 = (struct Node**)malloc(HT->buffer_size * sizeof(struct Node*));
    for (uint32_t i = 0; i < HT->buffer_size; ++i)
        arr2[i] = NULL;
    swap(&HT->arr, &arr2);
    for (uint32_t i = 0; i < HT->buffer_size; ++i)
    {
        if (arr2[i] && arr2[i]->state)
            HashTable_Add(HT, arr2[i]->value);
    }
    // удаление предыдущего массива
    for (uint32_t i = 0; i < HT->buffer_size; ++i)
        if (arr2[i])
            free(arr2[i]);
    free(arr2);
}

void* HashTable_Find(struct HashTable* HT, void* value)
{
    uint32_t h1 = HT->hash1(value, HT->buffer_size); // значение, отвечающее за начальную позицию
    uint32_t i = 0;
    while (HT->arr[h1] != NULL && i < HT->buffer_size)
    {
        if (HT->compare(HT->arr[h1]->value, value) && HT->arr[h1]->state)
            return HT->arr[h1]->value; // такой элемент есть
        h1 = rehash(h1, HT->buffer_size);
        ++i; // если у нас i >=  buffer_size, значит мы уже обошли абсолютно все ячейки, именно для этого мы считаем i, иначе мы могли бы зациклиться.
    }
    return NULL;
}

bool HashTable_Remove(struct HashTable* HT, void* value)
{
    uint32_t h1 = HT->hash1(value, HT->buffer_size);
    uint32_t i = 0;
    while (HT->arr[h1] != NULL && i < HT->buffer_size)
    {
        if (HT->compare(HT->arr[h1]->value, value) && HT->arr[h1]->state)
        {
            HT->arr[h1]->state = false;
            --(HT->size);
            return true;
        }
        h1 = rehash(h1, HT->buffer_size);
        ++i;
    }
    return false;
}

bool HashTable_Add(struct HashTable* HT, void* value)
{
    if (HT->size >= MAX_SIZE_CACHE)
        return false;
    if (HT->size + 1 > (uint32_t)(rehash_size * HT->buffer_size))
        HashTable_resize(HT);
    else if (HT->size_all_non_nullptr > 2 * HT->size)
        HashTable_Rehash(HT); // происходит рехеш, так как слишком много deleted-элементов
    uint32_t h1 = HT->hash1(value, HT->buffer_size);
    uint32_t i = 0;
    int first_deleted = -1; // запоминаем первый подходящий (удаленный) элемент
    while (HT->arr[h1] != NULL && i < HT->buffer_size)
    {
        if (HT->compare(HT->arr[h1]->value, value) && HT->arr[h1]->state)
            return false; // такой элемент уже есть, а значит его нельзя вставлять повторно
        if (!HT->arr[h1]->state && first_deleted == -1) // находим место для нового элемента
            first_deleted = h1;
        h1 = rehash(h1, HT->buffer_size);
        ++i;
    }
    if (first_deleted == -1) // если не нашлось подходящего места, создаем новый Node
    {
        HT->arr[h1] = Node_create(value);
        ++(HT->size_all_non_nullptr); // так как мы заполнили один пробел, не забываем записать, что это место теперь занято
    }
    else
    {
        HT->arr[first_deleted]->value = value;
        HT->arr[first_deleted]->state = true;
    }
    ++(HT->size); // и в любом случае мы увеличили количество элементов
    return true;
}

void swap(struct Node*** el1, struct Node*** el2){
    struct Node** tmp = *el1;
    *el1 = *el2;
    *el2 = tmp;
}

uint32_t rehash(uint32_t hash, uint32_t size) {
    // Используем другое простое число для вторичного хэша
    uint32_t new_hash = hash * 0x85ebca6b; // простое число
    new_hash ^= new_hash >> 16;
    new_hash *= 0xc2b2ae35;
    new_hash ^= new_hash >> 13;
    
    return (new_hash % (size - 1)) + 1;
}