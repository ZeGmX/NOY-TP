/*
 * test_data_overflow.c
 * Fills a big array to see if the paging systems works
 * when data becomes too big to fit in memory
 */
// Nachos system calls
#include "userlib/syscall.h"
#include "userlib/libnachos.h"

#define SIZE 1024
int big_array[SIZE];

int main() {

  int i = 0;
  for (; i < SIZE ; i++) {
    if (! (i % 50)) n_printf("%d\n", i);
    big_array[i] = i;
  }

  for (i = 0 ; i < SIZE ; i++) {
    if (big_array[i] != i) {
      n_printf("Error");
      Halt();
    }
  }
  return 0;
}
