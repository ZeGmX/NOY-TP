/* test.c
 * Test for a semaphore with several P before V
 */

// Nachos system calls
#include "userlib/syscall.h"
#include "userlib/libnachos.h"

SemId s;

void f1() {
  n_printf("Entering f1\n");
  P(s);
  n_printf("Exiting f1\n");
}

void f2() {
  n_printf("Entering f2\n");
  V(s);
  V(s);
  V(s);
  n_printf("Exiting f2\n");
}

int main() {
  s = SemCreate("mySema", 0);

  threadCreate("t1", &f1);
  threadCreate("t2", &f1);
  threadCreate("t3", &f1);
  threadCreate("t7", &f2);

  return 0;
}
