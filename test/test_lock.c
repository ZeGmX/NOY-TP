/* test_semaphore.c
 *
 *
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
  while (true) {
    LockAcquire(mutex);
//    n_printf("Filling with item %d\n", item);
    int i;
    for (i = 0 ; i < SIZE ; i++) buff[i] = item;
    LockRelease(mutex);
//    n_printf("Done\n");
    item++;
  }
}

void check() {
  while (true) {
    n_printf("helloo\n");
    int item = buff[0];
    int i;
    LockAcquire(mutex);
    for (i = 0 ; i < SIZE ; i++) {
      if (buff[i] != item) {
        n_printf("Error using lock, got items %d and %d\n", item, buff[i]);
        break;
      }
    }
    LockRelease(mutex);
    if (i == SIZE) n_printf("Successfully filled with item: %d\n", item);
  }
}

int main() {
  ThreadId id_filler = threadCreate("filler", &fill);
  ThreadId id_checker = threadCreate("checker", &check);

  n_printf("Filler id: %d, Checker id: %d\n", id_filler, id_checker);

  return 0;
}
