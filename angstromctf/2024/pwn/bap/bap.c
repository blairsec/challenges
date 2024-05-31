#include <stdio.h>

void main() {
    setbuf(stdout, NULL);

    char buf[16];

    printf(": ");
    gets(buf);
    printf(buf);
}