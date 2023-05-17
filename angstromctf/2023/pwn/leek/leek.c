#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/random.h>

int N = 100;

void win(){
    char flag[128];
    
    FILE *file = fopen("flag.txt","r");
    if (!file) {
        puts("Error: missing flag.txt.");
        exit(1);
    }

    fgets(flag, 128, file);
    puts(flag);
}

void input(char* addr){
    char buf[0x500];
    fgets(buf, 0x500, stdin);
    memcpy(addr, buf, strlen(buf));
}

void main(){
    srand(time(NULL));

    setbuf(stdout, NULL);
    setbuf(stdin, NULL);
    gid_t gid = getegid();
    setresgid(gid, gid, gid);
    
    puts("I dare you to leek my secret.");
    for(int i = 0; i < N; ++i){
        int sz = 0x10;
        char* ptr = malloc(sz);

        char* secret = malloc(0x20);
        memset(secret, 0, 0x20);
        getrandom(secret, 0x20, 0);

        for (int i = 0; i < 0x20; ++i){
            if(secret[i] == 0 || secret[i] == 0xa) secret[i] = 1;
        }

        printf("Your input (NO STACK BUFFER OVERFLOWS!!): ");
        
        input(ptr);

        printf(":skull::skull::skull: bro really said: ");
        puts(ptr);

        printf("So? What's my secret? ");

        char buf[0x21];
        fgets(buf, 0x21, stdin);

        if (strncmp(secret, buf, 0x20) != 0) {
            puts("Wrong!");
            exit(-1);
        }

        puts("Okay, I'll give you a reward for guessing it.");
        printf("Say what you want: ");

        gets(ptr);

        puts("Hmm... I changed my mind.");

        free(secret);
        free(ptr);

        puts("Next round!");
    }

    puts("Looks like you made it through.");
    win();
}