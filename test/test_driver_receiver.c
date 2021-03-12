/*
 * test_driver_receiver.c
 *
 * Testing our driver implementation (compatible with both acia modes)
 * A simple synchronization between two processus
 *
 * This is the receiver part
 */


// Nachos system calls
#include "userlib/syscall.h"
#include "userlib/libnachos.h"


int main() {
  char answer[128];

  TtyReceive(answer, 128);
  if (n_strcmp(answer, "SYN") != 0) {
    TtySend("NAK");
    Halt();
  }

  TtySend("SYN/ACK");
  TtyReceive(answer, 128);
  if (n_strcmp(answer, "ACK") != 0) {
    TtySend("NAK");
    Halt();
  }

  int n = TtyReceive(answer, 128);
  n_printf("Received message: %s\n", answer);

  Halt();

  return 0;
}
