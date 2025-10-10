#ifndef LIST_H
#define LIST_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

typedef struct list_elem {
    void* data;               // Данные
    struct list_elem* next; // Указатель на следующий элемент
} list_elem;

// Структура списка
typedef struct {
    list_elem* head;    // Указатель на первый элемент
    list_elem* tail;    // Указатель на последний элемент
    size_t size;        // Количество элементов
} list;

list* list_create(void);
void list_destroy(list* lst);
bool list_is_empty(const list* lst);
size_t list_size(const list* lst);

void list_push_front(list* lst, void* value);
void list_push_back(list* lst, void* value);
void* list_pop_front(list* lst);
void* list_pop_back(list* lst);

void list_insert(list* lst, size_t index, void* value);
void* list_remove(list* lst, size_t index);
void* list_get(const list* lst, size_t index);
void list_set(list* lst, size_t index, void* value);

bool list_contains(const list* lst, void* value);
size_t list_find(const list* lst, void* value);
void list_clear(list* lst);

void list_print(const list* lst);


#endif