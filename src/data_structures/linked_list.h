#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <stdlib.h>
typedef struct 
{
    void *data;
    struct ListNode *next;
} ListNode;


typedef struct
{
    ListNode *head;
    int size;
} LinkedList;
LinkedList *create_linked_list();
void list_insert(LinkedList *list, void *data);
void list_remove(LinkedList *list, void *data);
void *list_find(LinkedList *list, void *data);
void free_linked_list(LinkedList *list);

#endif
