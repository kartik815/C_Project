#include "globals.h"
#include <stdlib.h>

int* convert_to_unique_code(int num) {
  int *codes = (int *)malloc(6 * sizeof(int));

  for (int i = 0; i < 6; i++) {
    codes[i] = -1;
  }

  if ((num & 64) == 0) {
    if ((num & 1) != 0) {
      codes[0] = 0;
    }
    if ((num & 2) != 0) {
      codes[1] = 1;
    }

    if ((num & 4) != 0) {
      codes[2] = 2;
    }
    if ((num & 8) != 0) {
      codes[3] = 3;
    }

    if ((num & 16) != 0) {
      codes[4] = 4;
    }

    if ((num & 32) != 0) {
      codes[5] = 5;
    }

  } else {
    if ((num & 1) != 0) {
      codes[0] = 6;
    }
    if ((num & 2) != 0) {
      codes[1] = 7;
    }

    if ((num & 4) != 0) {
      codes[2] = 8;
    }
    if ((num & 8) != 0) {
      codes[3] = 9;
    }

    if ((num & 16) != 0) {
      codes[4] = 10;
    }

    if ((num & 32) != 0) {
      codes[5] = 11;
    }
  }

  return codes;
}
