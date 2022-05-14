#include <stdio.h>
#include <string.h>

int main(void) {
    printf("Password: ");
    fflush(stdout);
    char inp[256];
    fgets(inp, 256, stdin);
    inp[strcspn(inp, "\n")] = '\0';
    if (strcmp(inp, "fireworks")) {
        puts("Sorry, no flag for you");
    } else {
        FILE* flagfile = fopen("flag.txt", "r");
        char flag[256];
        fgets(flag, 256, flagfile);
        puts(flag);
    }
    return 0;
}
