
#include "queue.h"
#include <stdio.h>

int main() {
  Queue que, *temp_pointer = que_init();
  if (!temp_pointer) {
    printf("Can't create pointer\n");
    return -1;
  }
  que = *temp_pointer;
  que_enqueue(&que, 10);
  que_enqueue(&que, 20);
  que_enqueue(&que, 30);
  printf("%d\n", que_dequeue(&que));
  printf("%d\n", que_dequeue(&que));
  printf("%d\n", que_dequeue(&que));
  return 0;
}
