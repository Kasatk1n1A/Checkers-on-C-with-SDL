#include "hash_table.h"

#include <>

static void swap(void* el1, void* el2);

static struct Node* Node_create(const void* value);
static void Node_delete(struct Node* Node);

struct Node* Node_create(const void* value){
    struct Node* Node = (struct Node*)malloc(sizeof(struct Node));
    Node->value = value;
    Node->state = true;
    return Node;
}

void Node_delete(struct Node* Node){
    free(Node);
}

struct HashTable{
    struct Node** arr; // соответственно в массиве будут хранится структуры Node*
    int size; // сколько элементов у нас сейчас в массиве (без учета deleted)
    int buffer_size; // размер самого массива, сколько памяти выделено под хранение нашей таблицы
    int size_all_non_nullptr; // сколько элементов у нас сейчас в массиве (с учетом deleted)
}

static const int default_size = 8;
static const double rehash_size = 0.75;


struct HashTable* HashTable_create(void){
    struct HashTable* HashTable = (HashTable*)malloc(sizeof(struct HashTable));
    HashTable->buffer_size = default_size;
    HashTable->size = 0;
    HashTable->size_all_non_nullptr = 0;
    HashTable->arr = (struct Node**)malloc(HashTable->buffer_size * sizeof(struct Node*));
    for (int i = 0; i < buffer_size; ++i)
        arr[i] = NULL; // заполняем nullptr - то есть если значение отсутствует, и никто раньше по этому адресу не обращался
}

void HashTable_delete(struct HashTable* HashTable){
    for (int i = 0; i < buffer_size; ++i)
        if (arr[i])
            Node_delete(arr[i]);
    free(arr);
    free(HashTable);
}

void HashTable_resize(struct HashTable* HT){
    int past_buffer_size = HT->buffer_size;
    HT->buffer_size *= 2;
    HT->size_all_non_nullptr = 0;
    HT->size = 0;
    struct Node** arr2 = (struct Node**)malloc(buffer_size * sizeof(struct Node*));
    for (int i = 0; i < HT->buffer_size; ++i)
        arr2[i] = NULL;
    swap(arr, arr2);
    for (int i = 0; i < past_buffer_size; ++i)
    {
        if (arr2[i] && arr2[i]->state)
            HastTable_Add(HT, arr2[i]->value); // добавляем элементы в новый массив
    }
    // удаление предыдущего массива
    for (int i = 0; i < past_buffer_size; ++i)
        if (arr2[i])
            free(arr2[i])
    free(arr2);
}

void Rehash(struct HashTable* HT)
{
    HT->size_all_non_nullptr = 0;
    HT->size = 0;
    struct Node** arr2 = (struct Node**)malloc(HT->buffer_size * sizeof(struct Node*));
    for (int i = 0; i < HT->buffer_size; ++i)
        arr2[i] = NULL;
    swap(arr, arr2);
    for (int i = 0; i < HT->buffer_size; ++i)
    {
        if (arr2[i] && arr2[i]->state)
            HashTable_Add(arr2[i]->value);
    }
    // удаление предыдущего массива
    for (int i = 0; i < HT->buffer_size; ++i)
        if (arr2[i])
            free(arr2[i]);
    free(arr2);
}

bool Find(struct HashTable* HT, void* value, int (*hash1)(void*, int), int (*hash2)(void*, int), bool (*compare)(void*, void*))
{
    int h1 = hash1(value, buffer_size); // значение, отвечающее за начальную позицию
    int h2 = hash2(value, buffer_size); // значение, ответственное за "шаг" по таблице
    int i = 0;
    while (arr[h1] != NULL && i < HT->buffer_size)
    {
        if (compare(arr[h1]->value, value) && arr[h1]->state)
            return true; // такой элемент есть
        h1 = (h1 + h2) % HT->buffer_size;
        ++i; // если у нас i >=  buffer_size, значит мы уже обошли абсолютно все ячейки, именно для этого мы считаем i, иначе мы могли бы зациклиться.
    }
    return false;
}

bool Remove(struct HashTable* HT, void* value, int (*hash1)(void*, int), int (*hash2)(void*, int), bool (*compare)(void*, void*))
{
    int h1 = hash1(value, HT->buffer_size);
    int h2 = hash2(value, HT->buffer_size);
    int i = 0;
    while (arr[h1] != NULL && i < HT->buffer_size)
    {
        if (compare(arr[h1]->value, value) && arr[h1]->state)
        {
            arr[h1]->state = false;
            --(HT->size);
            return true;
        }
        h1 = (h1 + h2) % HT->buffer_size;
        ++i;
    }
    return false;
}

bool Add(struct HashTable* HT, void* value, int (*hash1)(void*, int), int (*hash2)(void*, int), bool (*compare)(void*, void*))
{
    if (HT->size + 1 > int(HT->rehash_size * HT->buffer_size))
        HashTable_Resize();
    else if (HT->size_all_non_nullptr > 2 * HT->size)
        HashTable_Rehash(); // происходит рехеш, так как слишком много deleted-элементов
    int h1 = hash1(value, HT->buffer_size);
    int h2 = hash2(value, HT->buffer_size);
    int i = 0;
    int first_deleted = -1; // запоминаем первый подходящий (удаленный) элемент
    while (arr[h1] != NULL && i < HT->buffer_size)
    {
        if (compare(arr[h1]->value, value) && arr[h1]->state)
            return false; // такой элемент уже есть, а значит его нельзя вставлять повторно
        if (!arr[h1]->state && first_deleted == -1) // находим место для нового элемента
            first_deleted = h1;
        h1 = (h1 + h2) % HT->buffer_size;
        ++i;
    }
    if (first_deleted == -1) // если не нашлось подходящего места, создаем новый Node
    {
        arr[h1] = Node_create(value);
        ++(HT->size_all_non_nullptr); // так как мы заполнили один пробел, не забываем записать, что это место теперь занято
    }
    else
    {
        arr[first_deleted]->value = value;
        arr[first_deleted]->state = true;
    }
    ++(HT->size); // и в любом случае мы увеличили количество элементов
    return true;
}

void swap(void* el1, void* el2){
    void* tmp = el1;
    el1 = el2;
    el2 = tmp;
}
