#include <stdio.h>
#include <stdlib.h>

void main(){
    char buf[100];

    setbuf(stdin, NULL);
    setbuf(stdout, NULL);

    fgets(buf, 100, stdin);

    if (strncmp(buf, "actf{ive_be3n_checkm4ted_21d1b2cebabf983f}", 42) == 0){
        puts("Correct!");
    } else {
        puts("Wrong!");
    }
}