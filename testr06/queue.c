#include <stdlib.h>

#include "queue.h"

Queue *queue_construct()
{
    Queue *queue = (Queue *)malloc(sizeof(Queue));

    queue->v = vector_construct();
    queue->size = 0;

    return queue;
}

void queue_push(Queue *queue, data_type val)
{
    vector_push_back(queue->v, val);

    queue->size++;
}

data_type queue_pop(Queue *queue)
{
    data_type val = vector_pop_front(queue->v);

    queue->size--;

    return val;
}

bool queue_empty(Queue *queue)
{
    if (queue->size == 0)
        return 1;

    return 0;
}

void queue_destroy(Queue *queue)
{
    vector_destroy(queue->v);
    free(queue);
}