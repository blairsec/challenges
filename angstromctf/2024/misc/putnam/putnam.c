#include <stdio.h>
#include <stdlib.h>

int main(void) {
    char inp[256];
    puts("514 + 97 = ?");
    fflush(stdout);
    fgets(inp, 256, stdin);
    if (atoi(inp) == 611) {
        puts("You succeeded! The flag is actf{just_a_tad_easier_than_the_actual_putnam} :D");
    } else {
        puts("You failed! No flag :(");
    }
    return 0;
}
