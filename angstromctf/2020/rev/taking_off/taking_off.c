#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#define FLAGSIZE 128

char desired[] = {90, 70, 79, 75, 89, 79, 10, 77, 67, 92, 79, 10, 76, 70, 75, 77, 42};

void print_flag() {
    gid_t gid = getegid();
    setresgid(gid, gid, gid);
    FILE *file = fopen("flag.txt", "r");
    char flag[FLAGSIZE];
    if (file == NULL) {
        printf("Cannot read flag file.\n");
        exit(1);
    }
    fgets(flag, FLAGSIZE, file);
    printf("%s", flag);
}

int string_to_int(char* str, int* num) {
    sscanf(str, "%d", num);
}

int is_invalid(int num) {
    return num < 0 || num >= 10;
}

int main(int argc, char* argv[]) {
    puts("So you figured out how to provide input and command line arguments.");
    puts("But can you figure out what input to provide?");
    if (argc != 5) {
        puts("Make sure you have the correct amount of command line arguments!");
        return 1;
    }
    int n1, n2, n3;
    string_to_int(argv[1], &n1);
    string_to_int(argv[2], &n2);
    string_to_int(argv[3], &n3);
    if (is_invalid(n1) || is_invalid(n2) || is_invalid(n3) || n2 * 100 + n1 * 10 + n3 != 932 || strcmp(argv[4], "chicken")) {
        puts("Don't try to guess the arguments, it won't work.");
        return 1;
    }
    puts("Well, you found the arguments, but what's the password?");
    char inp[128];
    fgets(inp, 128, stdin);
    char* newline = strchr(inp, '\n');
    if (newline) {
        *newline = 0;
    }
    int len = strlen(inp);
    for (int i = 0; i <= len; i ++) {
        if ((inp[i] ^ 42) != desired[i]) {
            puts("I'm sure it's just a typo. Try again.");
            return 1;
        }
    }
    puts("Good job! You're ready to move on to bigger and badder rev!");
    print_flag();
}