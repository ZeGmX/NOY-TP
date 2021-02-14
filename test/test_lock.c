/* test_lock.c
 * Simple program to check if our lock implementation works correctly
 */

// Nachos system calls
#include "userlib/syscall.h"
#include "userlib/libnachos.h"

#include <stdbool.h>

#define SIZE 512

int buff[SIZE];

LockId mutex;

void fill() {
  int item = 0;
  int i;
  while (true) {
    LockAcquire(mutex);
    for (i = 0 ; i < SIZE ; i++) buff[i] = item;
    n_printf("Array is filled with item %d\n", item);
    LockRelease(mutex);
    item++;
  }
}

void check() {
  int item;
  int i;
  while (true) {
    LockAcquire(mutex);
    item = buff[0];
    for (i = 0 ; i < SIZE ; i++) {
      if (buff[i] != item) {
        n_printf("Error: array contains %d instead of %d at index %d\n",
                 buff[i], item, i);
        Halt();
      }
    }
    n_printf("Checked array is filled with item %d\n", item);
    LockRelease(mutex);
  }
}

int main() {
  mutex = LockCreate("lockMutexTestLock");
  ThreadId id_filler = threadCreate("filler", &fill);
  ThreadId id_checker = threadCreate("checker", &check);

  return 0;
}
