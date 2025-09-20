#include "queue.h"

/*
 * Initialize queue
 */
Queue *que_init() {
    Queue *que = malloc(sizeof(Queue));
    if (!que) {
        return NULL;
    }
    que->head = NULL;
    que->tail = NULL;

    InitializeCriticalSection(&que->mutex); // Windows mutex init
    return que;
}

/*
 * Enqueue (add element to tail)
 */
int que_enqueue(Queue *que, int data) {
    if (!que)
        return -1;

    struct Que_data *new_node = malloc(sizeof(struct Que_data));
    if (!new_node) {
        return -1;
    }

    new_node->data = data;
    new_node->next_node = NULL;

    EnterCriticalSection(&que->mutex);
    if (!que->head) {
        que->head = new_node;
        que->tail = new_node;
        LeaveCriticalSection(&que->mutex);
        return 0;
    }

    que->tail->next_node = new_node;
    que->tail = new_node;

    LeaveCriticalSection(&que->mutex);
    return 0;
}

/*
 * Dequeue (remove element from head)
 */
int que_dequeue(Queue *que) {
    if (!que || !que->head) {
        return -1;
    }

    EnterCriticalSection(&que->mutex);

    if (que->head == que->tail) {
        int res = que->head->data;
        free(que->head);
        que->head = NULL;
        que->tail = NULL;
        LeaveCriticalSection(&que->mutex);
        return res;
    }

    int res = que->head->data;
    struct Que_data *temp = que->head;

    que->head = que->head->next_node;
    LeaveCriticalSection(&que->mutex);
    free(temp);

    return res;
}

/*
 * Destroy the queue
 */
void que_destroy(Queue *que) {
    if (!que)
        return;

    EnterCriticalSection(&que->mutex);

    struct Que_data *current = que->head;
    while (current) {
        struct Que_data *next = current->next_node;
        free(current);
        current = next;
    }
    que->head = NULL;
    que->tail = NULL;

    LeaveCriticalSection(&que->mutex);
    DeleteCriticalSection(&que->mutex);  // Free system resources

    free(que);
}
