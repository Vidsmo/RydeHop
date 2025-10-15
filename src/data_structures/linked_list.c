#include "linked_list.h"
#include <stdlib.h>
#include <stdio.h>

LinkedList *create_linked_list()
{
    LinkedList *list=(LinkedList *)malloc(sizeof(LinkedList));
    if(list==NULL)
    {
        printf("memoery allocation failed\n");
        return NULL;
    }
    list->head=NULL;
    list->size=0;   
}

void list_insert(LinkedList *list, void *data)
{
    if(list==NULL)
    {
        printf("list is NULL\n");
        return;
    }
   ListNode *node=(ListNode *)malloc(sizeof(ListNode));
   if(node==NULL)
   {
    printf("memory allocation failed\n");
    return;
   }
    node->data=data;
    node->next=list->head;
list->head=node;
list->size++;
}
void list_remove(LinkedList *List,void *data)
{
    if(List==NULL)
    {
        printf("list is empty");
        return ;
    }
    ListNode *curr=List->head;
    ListNode *prev=NULL;
    while(curr!=NULL)
    {
        if(curr->data==data)
        {
            if(prev==NULL)
            {
                List->head=curr->next;
            }
            else
            {
                prev->next=curr->next;
            }
            free(curr);
            List->size--;
        }
        prev=curr;
        curr=curr->next;
    }
}
void *list_find(LinkedList *list,void *data)
{
    if(list==NULL)
    {
        printf("list is NULL\n");
        return NULL;
    }
    ListNode *curr=list->head;
    while(curr!=NULL)
    {
        if(curr->data==data)
        {
            return curr->data;
        }
        curr=curr->next;
    }
}
void free_linked_list(LinkedList *list)
{
    if(list==NULL)
    {
        return;
    }
    ListNode *curr=list->head;
    while(curr!=NULL)
    {
        ListNode *temp=curr;
        curr=curr->next;
        free(temp);
    }
    free(list);
}