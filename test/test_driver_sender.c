/*
 * test_driver_sender.c
 *
 * Testing our driver implementation (compatible with both acia modes)
 * A simple synchronization between two processus
 *
 * This is the sender part
 */


// Nachos system calls
#include "userlib/syscall.h"
#include "userlib/libnachos.h"


int main() {
  char answer[128];

  TtySend("SYN");
  TtyReceive(answer, 128);
  n_printf("Sender received message: %s\n", answer);
  if (n_strcmp(answer, "SYN/ACK") != 0) {
    TtySend("NAK");
    Halt();
  }

  TtySend("ACK");

  int i;
  // If we send two messages too fast, one may overwrite the other
  for (i = 0 ; i < 1000 ; i++) {;}

  int n = TtySend("Hello world!");
  n_printf("Actual size of the string sent: %d\n", n);

  Halt();

  return 0;
}
