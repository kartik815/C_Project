
/*
* Defining Queue and all its function in this header file.
*
* The queue will be used to store 
*
*
*/

#ifndef QUEUE_H
#define QUEUE_H 

#include <stdlib.h>
#include <pthread.h>

struct Que_data {
	int data;
	struct Que_data *next_node;

};

typedef struct Queue {
	struct Que_data *head;
	struct Que_data *tail;

	//
} Queue;


Queue* que_init();
int que_enqueue(Queue *que, int data);
int que_dequeue(Queue *que);
#endif
