#include "cases.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char inp[256];

void WIN_NODE(char* i) {
    if (*i == '\0') {
        printf("The flag is actf{%s}\n", inp);
        exit(0);
    } else {
        puts("That's not the password...");
        exit(1);
    }
}

int main(void) {
    printf("Enter the password: ");
    fflush(stdout);
    fgets(inp, 256, stdin);
    inp[strcspn(inp, "\n")] = '\0';
    START_NODE(inp);
    puts("That's not the password...");
    return 1;
}
