#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#define FLAGSIZE 128

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

int main(int argc, char* argv[]) {
    puts("Congratulations on running the binary!");
    puts("Now there are a few more things to tend to.");
    puts("Please type \"give flag\" (without the quotes).");
    char inp[128];
    fgets(inp, 128, stdin);
    char* newline = strchr(inp, '\n');
    if (newline) {
        *newline = 0;
    }
    if(strcmp(inp, "give flag")) {
        printf("You entered \"%s\", not \"give flag\". Please try again.\n", inp);
        return 1;
    }
    puts("Good job!");
    if (argc < 2) {
        puts("Now run the program with a command line argument of \"banana\" and you'll be done!");
        return 1;
    }
    if (strcmp(argv[1], "banana")) {
        printf("You provided \"%s\", not \"banana\". Please try again.\n", argv[1]);
        return 1;
    }
    puts("Well I think it's about time you got the flag!");
    print_flag();
}