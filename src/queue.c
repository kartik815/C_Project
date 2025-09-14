#include "queue.h"

Queue *que_init() {
  Queue *que = malloc(sizeof(Queue));
  if (!que) {
    return NULL;
  }
  que->head = NULL;
  que->tail = NULL;
  return que;
}

int que_enqueue(Queue *que, int data) {
	if (!que) return -1;

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
