/* test_lock.c
 * Simple program to check if our lock implementation works correctly
 */

// Nachos system calls
#include "userlib/syscall.h"
#include "userlib/libnachos.h"

#include <stdbool.h>

#define SIZE 512

int buff[SIZE];

LockId mutex;  // Protects the buffer


/*
 * Main function for the filler
 * It takes the lock to fill a buffer with the same item in every cell
 */
void fill() {
  int item = 0;  // item to fill the  buffer with
  int i;
  while (true) {
    LockAcquire(mutex);  // begining critical section
    for (i = 0 ; i < SIZE ; i++) buff[i] = item;
    n_printf("Array is filled with item %d\n", item);
    LockRelease(mutex);  // end of critical section
    item++;
  }
}

/*
 * Main function for the checker
 * It checks if every item in the buffer is the same
 * (This ensures that the checker couldn't take the lock
 * while the filler had it)
 */
void check() {
  int item; // common item to every cell in the buffer
  while (true) {
    LockAcquire(mutex);  // Begining criticall section
    item = buff[0];

    int i;  // loop index
    for (i = 0 ; i < SIZE ; i++) {
      if (buff[i] != item) {
        n_printf("Error: array contains %d instead of %d at index %d\n",
                 buff[i], item, i);
        Halt();
      }
    }
    LockRelease(mutex);  // end of critical section
    n_printf("Checked array is filled with item %d\n", item);
  }
}

int main() {
  mutex = LockCreate("lockMutexTestLock");

  threadCreate("filler", &fill);
  threadCreate("checker", &check);

  return 0;
}
