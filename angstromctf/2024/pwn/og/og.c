#include <stdio.h>

void go() {

  setbuf(stdin, NULL);
  setbuf(stdout, NULL);
  setbuf(stderr, NULL);

  char name[0x20];

  printf("kill $PPID;");

  fgets(name, 0x42, stdin);

  printf("Gotta go. See you around, ");

  printf(name);
}

int main() { go(); }
