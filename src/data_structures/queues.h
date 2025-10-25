#ifndef QUEUE_H
#define QUEUE_H

#include "linked_list.h"

typedef struct
{
    LinkedList *list;
} Queue;

Queue *create_queue();
void enqueue(Queue *queue, void *data);
void *dequeue(Queue *queue);
void free_queue(Queue *queue);

#endif