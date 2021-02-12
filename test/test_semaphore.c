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

SemId filled, empty;
int buff[SIZE];
int indexBegin, indexEnd;

void addItem(int item) {
  buff[indexEnd] = item;
  indexEnd = (indexEnd + 1) % SIZE;
  if (indexEnd == indexBegin) n_printf("overlapping");
}

int takeItem() {
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
    n_printf("Proucter put item: %d\n", item);
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
  indexBegin = 0;
  indexEnd = 0;

  ThreadId id_cons = threadCreate("consumer", &consom);
  ThreadId id_prod = threadCreate("producer", &prod);

  n_printf("Prod id: %d\nCons id: %d\n", id_prod, id_cons);

  return 0;
}
