#include "list.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

/* Create empty list */
list* list_create(void) {
    list* lst = (list*)malloc(sizeof(list));
    if (!lst) return NULL;
    
    lst->head = NULL;
    lst->tail = NULL;
    lst->size = 0;
    return lst;
}

/* Destroy list */
void list_destroy(list* lst) {
    if (!lst) return;
    
    list_clear(lst);
    free(lst);
}

/* checking for emptiness */
bool list_is_empty(const list* lst) {
    return lst ? (lst->size == 0) : true;
}

size_t list_size(const list* lst) {
    return lst ? lst->size : 0;
}

/* Add in begin */
void list_push_front(list* lst, void* value) {
    if (!lst) return;
    
    list_elem* new_elem = (list_elem*)malloc(sizeof(list_elem));
    if (!new_elem) return;
    
    new_elem->data = value;
    new_elem->prev = NULL;
    new_elem->next = lst->head;
    
    lst->head = new_elem;
    if (!lst->tail) {
        lst->tail = new_elem;
    }
    lst->size++;
}

/* Add in end */
void list_push_back(list* lst, void* value) {
    if (!lst) return;
    
    list_elem* new_elem = (list_elem*)malloc(sizeof(list_elem));
    if (!new_elem) return;
    
    new_elem->data = value;
    new_elem->prev = lst->tail;
    new_elem->next = NULL;
    
    if (lst->tail) {
        lst->tail->next = new_elem;
    } else {
        lst->head = new_elem;
    }
    lst->tail = new_elem;
    lst->size++;
}

/* Remove from begin */
void* list_pop_front(list* lst) {
    if (!lst || !lst->head) return 0;
    
    list_elem* temp = lst->head;
    void* value = temp->data;
    
    lst->head = lst->head->next;
    lst->head->prev = NULL;
    if (!lst->head) {
        lst->tail = NULL;
    }
    
    free(temp);
    lst->size--;
    return value;
}

/* Remove from end */
void* list_pop_back(list* lst) {
    if (!lst || !lst->tail) return NULL;
    
    list_elem* temp = lst->tail;
    void* value = temp->data;
    
    // Обновляем указатель tail
    lst->tail = lst->tail->prev;
    
    if (lst->tail) {
        // Если есть предыдущий элемент, обнуляем его next
        lst->tail->next = NULL;
    } else {
        // Если список стал пустым, обнуляем head
        lst->head = NULL;
    }
    
    free(temp);
    lst->size--;
    return value;
}

/* Add with index */
void list_insert(list* lst, size_t index, void* value) {
    if (!lst || index > lst->size) return;
    
    if (index == 0) {
        list_push_front(lst, value);
        return;
    }
    if (index == lst->size) {
        list_push_back(lst, value);
        return;
    }
    
    list_elem* new_elem = (list_elem*)malloc(sizeof(list_elem));
    if (!new_elem) return;
    
    new_elem->data = value;
    
    // Поиск с начала
    list_elem* current = lst->head;
    for (size_t i = 0; i < index - 1; i++) {
        current = current->next;
    }
    
    // Устанавливаем связи для нового элемента
    new_elem->prev = current;
    new_elem->next = current->next;
    
    // Обновляем связи соседних элементов
    current->next->prev = new_elem;
    current->next = new_elem;
    
    lst->size++;
}

/* Remove with index */
void* list_remove(list* lst, size_t index) {
    if (!lst || index >= lst->size) return NULL;
    
    if (index == 0) return list_pop_front(lst);
    if (index == lst->size - 1) return list_pop_back(lst);
    
    // Поиск с начала
    list_elem* current = lst->head;
    for (size_t i = 0; i < index; i++) {
        current = current->next;
    }
    
    void* value = current->data;
    
    // Перенаправляем связи, исключая удаляемый элемент
    current->prev->next = current->next;
    current->next->prev = current->prev;
    
    free(current);
    lst->size--;
    return value;
}

/* Get element with index */
void* list_get(const list* lst, size_t index) {
    if (!lst || index >= lst->size) return 0;
    
    list_elem* current = lst->head;
    for (size_t i = 0; i < index; i++) {
        current = current->next;
    }
    return current->data;
}

/* Set value with index */
void list_set(list* lst, size_t index, void* value) {
    if (!lst || index >= lst->size) return;
    
    list_elem* current = lst->head;
    for (size_t i = 0; i < index; i++) {
        current = current->next;
    }
    current->data = value;
}

/* Check element in list */
bool list_contains(const list* lst, void* value) {
    if (!lst) return false;
    
    list_elem* current = lst->head;
    while (current) {
        if (current->data == value) return true;
        current = current->next;
    }
    return false;
}

/* Find element`s index */
size_t list_find(const list* lst, void* value) {
    if (!lst) return (size_t)-1;
    
    list_elem* current = lst->head;
    size_t index = 0;
    while (current) {
        if (current->data == value) return index;
        current = current->next;
        index++;
    }
    return (size_t)-1;
}

list_elem* list_max(const list* lst, bool(*bigger)(list_elem*, list_elem*)){
    list_elem* result = lst->head;
    list_elem* curr = lst->head;

    while (curr){
        if (bigger(curr, result)){
            result = curr;
        }
        curr = curr->next;
    }

    return result;
}

/* Clear list */
void list_clear(list* lst) {
    if (!lst) return;
    
    list_elem* current = lst->head;
    while (current) {
        list_elem* next = current->next;
        free(current);
        current = next;
    }
    
    lst->head = NULL;
    lst->tail = NULL;
    lst->size = 0;
}

void list_for_each(list* lst, void* (*do_smth)(void*)){
    list_elem* curr = lst->head;

    for (size_t i = 0; i < list_size(lst); i++, curr = curr->next){
        do_smth(curr->data);
    }
}
/* Print list in stdout */
// void list_print(const list* lst) {
//     if (!lst) {
//         printf("List is NULL\n");
//         return;
//     }
    
//     printf("List [size=%zu]: ", lst->size);
//     list_elem* current = lst->head;
//     while (current) {
//         printf("%d", current->data);
//         if (current->next) printf(" -> ");
//         current = current->next;
//     }
//     printf("\n");
// }