#ifndef QUEUE_H
#define QUEUE_H
#include <windows.h>   // for CRITICAL_SECTION
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// ---------------------- DataPacket queue (linked list) ----------------------

// Packet structure
typedef struct Data_packet {
    uint32_t timestamp; // 4 bytes fixed
    uint8_t code;
} DataPacket;

// Linked-list node for DataPacket
typedef struct Node {
    DataPacket data;
    struct Node* next;
} Node;

// Function declarations
void sortedEnqueue(Node** front, Node** rear, DataPacket packet);
void printQueue(Node* front);
DataPacket deQueue(Node** front);   // <-- fixed (semicolon added)


// ---------------------- Integer queue (thread-safe) ----------------------

typedef struct Que_data {
    int data;
    struct Que_data *next_node;
} QueData;

typedef struct Queue {
    QueData *head;
    QueData *tail;
    CRITICAL_SECTION mutex;   // Windows equivalent of pthread_mutex_t
} Queue;

// Queue function declarations
Queue* que_init();
int que_enqueue(Queue *que, int data);
int que_dequeue(Queue *que);
void que_destroy(Queue *que);

#endif
