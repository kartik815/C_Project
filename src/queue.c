#include "queue.h"

// Insert packet in queue sorted by timestamp
void sortedEnqueue(Node** front, Node** rear, DataPacket packet) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->data = packet;
    newNode->next = NULL;

    // Empty queue
    if (*front == NULL) {
        *front = *rear = newNode;
        return;
    }

    // Insert at front
    if (packet.timestamp < (*front)->data.timestamp) {
        newNode->next = *front;
        *front = newNode;
        return;
    }

    // Insert in middle or end
    Node* current = *front;
    while (current->next != NULL && current->next->data.timestamp <= packet.timestamp) {
        current = current->next;
    }

    newNode->next = current->next;
    current->next = newNode;

    // Update rear if inserted at end
    if (newNode->next == NULL) {
        *rear = newNode;
    }
}

DataPacket deQueue(Node** front) {  // ← Take pointer to pointer
    DataPacket emptyPacket = {0};
    if (*front == NULL) {  // ← Dereference to check
        return emptyPacket;
    }

    Node* temp = *front;
    DataPacket packet = temp->data;
    *front = temp->next;  // ← This modifies the ORIGINAL pointer!
    free(temp);
    return packet;
}



// Print queue contents
void printQueue(Node* front) {
    Node* temp = front;
    while (temp != NULL) {
        printf("Timestamp: %u, Code: %u\n", temp->data.timestamp, temp->data.code);
        temp = temp->next;
    }
    printf("------\n");
}







/*


---------------------- Queue 2 functions ----------------------

*/





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