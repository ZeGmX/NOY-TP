/*
 *
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
  n_printf("%d\n", n);
  n_printf("%s %d\n", answer, n_strlen(answer));


  return 0;
}
