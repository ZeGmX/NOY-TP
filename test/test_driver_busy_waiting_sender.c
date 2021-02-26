/*
 *
 */


// Nachos system calls
#include "userlib/syscall.h"
#include "userlib/libnachos.h"


/*
 * TODO:
 * - Never put two "send" without a "receive"
 *  One may be overwriten by the other
 *
 * - Threads not ending
 *
 * - Counting of send/receive don't match
 */

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
  int i;
  for (i = 0 ; i < 1000 ; i++) {;}
  int n = TtySend("Hello world hello world!");
  n_printf("%d\n", n);

  return 0;
}
