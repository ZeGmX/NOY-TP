/*
 *
 *
 */
#include "userlib/syscall.h"
#include "userlib/libnachos.h"

#define SIZE 16

void sort(char* tab) {
  int i, j, key;
  for (j = 1; j < SIZE; j++)
  {
    key = tab[j];
    i = j - 1;
    while (i >= 0 && tab[i] > key)
    {
      tab[i + 1] = tab[i];
      i--;
    }
    tab[i + 1] = key;
  }
}


int main() {
  char buff[SIZE];
  char buff_read[SIZE];

  Create("numbers.txt", SIZE);
  OpenFileId idFile = Open("numbers.txt");

  Mmap(idFile, 512);

  char i;
  for (i = 0 ; i < SIZE ; i++) buff[i] = SIZE - i;
  int realSize = Write(buff, SIZE, idFile);

  if (realSize != SIZE) {
    n_printf("Write went wrong\n");
    Halt();
  }

  Close(idFile);
  idFile = Open("numbers.txt");

  realSize = Read(buff_read, SIZE, idFile);
  if (realSize != SIZE) {
    n_printf("Read went wrong\n");
    Halt();
  }

  Close(idFile);

  sort(buff_read);
  int j;
  for (j = 0 ; j < SIZE ; j++) {
    n_printf(" %d", buff_read[j]);
  }
  n_printf("\n");
  return 0;
}
