/*
 *
 */


// Nachos system calls
#include "userlib/syscall.h"
#include "userlib/libnachos.h"


int main() {
  char answer[128];

  TtySend("SYN");
  TtyReceive(answer, 128);
//  n_printf("Sender received message: %s\n", answer);
  if (n_strcmp(answer, "SYN/ACK") != 0) {
    TtySend("NAK");
    Halt();
  }

  TtySend("ACK");
//  n_printf("Sending hello world\n");
  TtySend("Hello world!");
//  n_printf("Done sending\n");

  return 0;
}
