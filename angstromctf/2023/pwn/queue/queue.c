#include <stdio.h>
#include <stdlib.h>

void main(){
    setbuf(stdout, NULL);
    gid_t gid = getegid();
    setresgid(gid, gid, gid);

    char flag[128];
    
    FILE *file = fopen("flag.txt","r");
    if (!file) {
        puts("Error: missing flag.txt.");
        exit(1);
    }

    fgets(flag, 128, file);

    char input[48];

    printf("What did you learn in class today? ");
    
    fgets(input, 48, stdin);

    printf("Oh nice, ");
    printf(input);
    printf("sounds pretty cool!");
}