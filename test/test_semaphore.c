/* test_semaphore.c
 * Simple producer-consumer implementation
 * using semaphore
 * It is useful to test the validity of the
 * our thread and semaphore implementation
 */

// Nachos system calls
#include "userlib/syscall.h"
#include "userlib/libnachos.h"

#include <stdbool.h>

#define SIZE 512

SemId filled, empty, mutex;
int buff[SIZE];
int indexBegin, indexEnd;
int count_empty;

void addItem(int item) {
  P(mutex);
  if (count_empty == 0)
  {
    n_printf("No items to add!\n");
    Halt();
  }
  count_empty--;
  V(mutex);
  buff[indexEnd] = item;
  indexEnd = (indexEnd + 1) % SIZE; 
}

int takeItem() {
  P(mutex);
  if (count_empty == SIZE)
  {
    n_printf("No items to take!\n");
    Halt();
  }
  count_empty++;
  V(mutex);
  int item = buff[indexBegin];
  indexBegin = (indexBegin + 1) % SIZE;
  return item;
}

void prod() {
  int item = 0;
  while (true) {
    item++;
    P(empty);
    addItem(item);
    V(filled);
    n_printf("Producter put item: %d\n", item);
  }
}

void consom() {
  int item;
  while(true) {
    P(filled);
    item = takeItem();
    V(empty);
    n_printf("Consumer took item: %d\n", item);
  }
}

int main() {
  filled = SemCreate("semaFilled", 0);
  empty = SemCreate("semaEmpty", SIZE);
  mutex = SemCreate("semaMutex", 1);
  indexBegin = 0;
  indexEnd = 0;
  count_empty = SIZE;

  ThreadId id_prod = threadCreate("producer", &prod);
  ThreadId id_cons = threadCreate("consumer", &consom);

  return 0;
}
