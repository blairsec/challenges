#include <stdio.h>
#include <stdlib.h>

void win1(int a, int b){
    if(a != 0x1337 && b != 0x4141) return;
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

    puts("Nice!");

    puts("Now you need to call the win1 function with the correct arguments.");
    printf("Your input: ");
    
    gets(buf);
}