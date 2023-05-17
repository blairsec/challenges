#include <stdio.h>
#include <stdlib.h>

void win0(){
    char flag[128];
    
    FILE *file = fopen("flag.txt","r");
    if (!file) {
        puts("Error: missing flag.txt.");
        exit(1);
    }

    fgets(flag, 128, file);
    puts(flag);
}

void main(){
    setbuf(stdout, NULL);
    gid_t gid = getegid();
    setresgid(gid, gid, gid);

    char buf[48];

    puts("Welcome to gaga!");
    puts("This challenge is meant to guide you through an introduction to binary exploitation.");

    printf("\nRight now, you are on stage0. Your goal is to redirect program control to win0, which is at address %p.\n", &win0);

    printf("Your input: ");
    
    gets(buf);
}