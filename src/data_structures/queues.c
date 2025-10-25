#include "queues.h"
#include <stdlib.h>
Queue *create_queue()
{
    Queue *q=(Queue *)malloc(sizeof(Queue));
    if(q==NULL)
    {
        printf("memory allocation failed\n");
        return NULL;
    }
    q->list=create_linked_list();
    return q;
}

void enqueue(Queue *queue, void *data)
{
    if(queue==NULL)
    {
        printf("queue is NULL\n");
        return;
    }
    list_insert(queue->list,data);
}

void *dequeue(Queue *queue)
{
    if(queue==NULL || queue->list->head==NULL)
    {
        printf("queue is empty\n");
        return NULL;
    }
     list_remove(queue->list, queue->list->head->data);
}

void free_queue(Queue *queue)
{
    if(queue==NULL)
    {
        return;
    }
    free_linked_list(queue->list);
    free(queue);
}