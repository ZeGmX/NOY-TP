/*
 *
 */
// Nachos system calls
#include "userlib/syscall.h"
#include "userlib/libnachos.h"

#define SIZE 1000
int big_array[SIZE];

int main() {

  int i = 0;
  for (; i < SIZE ; i++) {
    if (! (i % 50)) n_printf("%d\n", i);
    big_array[i] = i;
  }
  return 0;
}
