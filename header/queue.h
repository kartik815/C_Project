
#ifndef QUEUE_H
#define QUEUE_H 

#include <stdlib.h>
#include <windows.h>   // for CRITICAL_SECTION

struct Que_data {
    int data;
    struct Que_data *next_node;
};

typedef struct Queue {
    struct Que_data *head;
    struct Que_data *tail;

    CRITICAL_SECTION mutex;   // Windows equivalent of pthread_mutex_t
} Queue;

Queue* que_init();
int que_enqueue(Queue *que, int data);
int que_dequeue(Queue *que);
void que_destroy(Queue *que);

#endif
