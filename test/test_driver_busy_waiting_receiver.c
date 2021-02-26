/*
 *
 */


// Nachos system calls
#include "userlib/syscall.h"
#include "userlib/libnachos.h"


int main() {
  char answer[128];

  TtyReceive(answer, 128);
//  n_printf("Receiver received message: %s\n", answer);
  if (n_strcmp(answer, "SYN") != 0) {
    TtySend("NAK");
    Halt();
  }

  TtySend("SYN/ACK");
  TtyReceive(answer, 128);
//  n_printf("Receiver received message: %s\n", answer);
  if (n_strcmp(answer, "ACK") != 0) {
    TtySend("NAK");
    Halt();
  }
//  n_printf("Last receive\n");
  TtyReceive(answer, 128);
//  n_printf("Done\n");
  n_printf("%s %d\n", answer, n_strlen(answer));


  return 0;
}
