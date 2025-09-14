#include "queue.h"

/*
 * This Fuction is used to create a pointer for queue and
 * return the pointer using malloc. its returns Null if
 * malloc couldn't allocate the memory
 */
Queue *que_init() {
  Queue *que = malloc(sizeof(Queue));
  if (!que) {
    return NULL;
  }
  que->head = NULL;
  que->tail = NULL;
  return que;
}

/*
 * This function create new node with the given and
 * adds it to the tail of queue.
 * Returns -1 if error occurs and returns 0 if
 * function is executed successfully.
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
  if (!que->head) {
    que->head = new_node;
    que->tail = new_node;
    return 0;
  }

  que->tail->next_node = new_node;
  que->tail = new_node;
  return 0;
}

/*
 * This function pop elements from the queue and
 * returns the data. If error occurs, the function
 * returns -1.
 */
int que_dequeue(Queue *que) {
  if (!que || !que->head) {
    return -1;
  }

  if (que->head == que->tail) {
    int res = que->head->data;
    free(que->head);
    que->head = NULL;
    que->tail = NULL;
    return res;
  }

  int res = que->head->data;
  struct Que_data *temp = que->head;

  que->head = que->head->next_node;
  free(temp);

  temp = NULL;

  return res;
}
