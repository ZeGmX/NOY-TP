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
// indexBegin: next index to take for the consumer
// indexEnd: next index to put an item for the producer
int count_empty;
// number of empty cells
// useful to make a difference when the two indices are the same:
// - When the buffer is full (the producer completed a loop)
// - When the buffer is empty (the consumer has taken everything)

void addItem(int item) {
  P(mutex);  // protecting the variable count_empty
  if (count_empty == 0)
  {
    // This should not happen if the semaphores work correctly
    n_printf("No items to add!\n");
    Halt();
  }
  count_empty--;
  V(mutex);  // end of critical section

  buff[indexEnd] = item;
  indexEnd = (indexEnd + 1) % SIZE; 
}

int takeItem() {
  P(mutex);  // protecting the variable count_empty
  if (count_empty == SIZE)
  {
    // This should not happen if the semaphores work correctly
    n_printf("No items to take!\n");
    Halt();
  }
  count_empty++;
  V(mutex);  // end of critical section

  int item = buff[indexBegin];
  indexBegin = (indexBegin + 1) % SIZE;
  return item;
}

/*
 * Main function for the producer, it produces an item
 * (1, 2, ...) and puts it into the buffer for the consumer
 * to take
 */
void prod() {
  int item = 0;  // item to put in the buffer

  while (true) {
    item++;

    P(empty);  // there is one less empty cell
    addItem(item);
    V(filled);  // there is one more filled cell

    n_printf("Producter put item: %d\n", item);
  }
}

/*
 * Main function for the consumer, it takes an item
 * and prints it
 */
void consom() {
  int item;  // item taken

  while(true) {
    P(filled);  // One less filled cell
    item = takeItem();
    V(empty);  // one more empty cell

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

  threadCreate("producer", &prod);
  threadCreate("consumer", &consom);

  return 0;
}
