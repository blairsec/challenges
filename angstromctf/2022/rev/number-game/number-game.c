#include <stdio.h>
#include <string.h>

int read_int() {
    int x;
    scanf("%d", &x);
    return x;
}

void print_flag() {
    FILE* flagfile = fopen("flag.txt", "r");
    if (flagfile == NULL) {
        puts("Can't read flag file. If this is happening on remote, contact an organizer.");
        return;
    }
    char flag[64];
    fgets(flag, 64, flagfile);
    flag[strcspn(flag, "\n")] = '\0';
    puts(flag);
}

int main(void) {
    puts("Welcome to clam's number game!");
    printf("Step right up and guess your first number: ");
    fflush(stdout);
    int n = read_int();
    if (n != 314159265) {
        puts("Sorry but you didn't win :(");
        return 1;
    }
    printf("That's great, but can you follow it up? ");
    fflush(stdout);
    int m = read_int();
    if (n + m != 513371337) {
        puts("Sorry but you didn't win :(");
        return 1;
    }
    char inp[64];
    puts("That was the easy part. Now, what's the 42nd number of the Maltese alphabet?");
    getchar();
    fgets(inp, 64, stdin);
    inp[strcspn(inp, "\n")] = '\0';
    if (strcmp(inp, "the airspeed velocity of an unladen swallow")) {
        puts("Ha! I knew I would get you there!");
        return 1;
    }
    puts("How... how did you get that? That reference doesn't even make sense...");
    puts("Whatever, you can have your flag I guess.");
    print_flag();
    return 0;
}
