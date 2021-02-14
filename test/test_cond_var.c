/* test_cond_var.c
 * testing our implementation of the condition variables
 * Several threads are waiting and when the users writes '1',
 * a broadcaster wakes them all up
 */

// Nachos system calls
#include "userlib/syscall.h"
#include "userlib/libnachos.h"

#include <stdbool.h>

#define SIZE 4

CondId cond;

ThreadId waitersids[SIZE];
ThreadId broadcasterId;


// Waiter main function
void waiter() {
  CondWait(cond);
  n_printf("Hey, i'm awake now!\n");
}

// Broadcaster main function
void broadcaster() {
  n_printf("Broadcaster is working\n");
  n_printf("Enter '1' to wake up the threads\n");
  while(n_read_int() != 1) n_printf("\nEnter '1' to wake up the threads\n");
  CondBroadcast(cond);  // Wakes the other threads
}


int main() {
  cond = CondCreate("conditionTest_cond_var");

  // Creating the waiter threads
  int i;
  for (i = 0 ; i < SIZE ; i++) {
    n_printf("loop index: %d\n", i);
    char name[32];
    int actualLen = n_snprintf(name, 32, "threadWaiter%d", i);

    if (actualLen < 0) {
      n_printf("n_snprintf failed\n");
      Halt();
    }
    waitersids[i] = threadCreate(name, &waiter);

  }
  // Creating the broadcaster thread
  broadcasterId = threadCreate("threadBroadcaster", &broadcaster);

  // waiting for the threads to end;
  for (i = 0 ; i < SIZE ; i++) Join(waitersids[i]);
  Join(broadcasterId);

  return 0;
}
