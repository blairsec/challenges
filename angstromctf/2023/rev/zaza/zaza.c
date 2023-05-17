#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

void xor_(char* buf1){
    char * key = "anextremelycomplicatedkeythatisdefinitelyuselessss";
    for(int i = 0; i < strlen(key); ++i){
        buf1[i] ^= key[i];
    }
}

void main(){
    setbuf(stdout, NULL);
    int x = 0;
    int y = 0;

    printf("I'm going to sleep. Count me some sheep: ");

    scanf("%d", &x);

    if (x != 0x1337){
        puts("That's not enough sheep!");
        exit(1);
    }

    printf("Nice, now reset it. Bet you can't: ");

    scanf("%d", &y);

    if (x * y == 1){
        printf("%d %d", y, x+y);
        puts("Not good enough for me.");
        exit(1);
    }

    char buf[64];
    puts("Okay, what's the magic word?");
    getchar();
    fgets(buf, 64, stdin);
    buf[strcspn(buf, "\n")] = '\0';

    xor_(buf);

    if(strncmp(buf, "\x32\x26\x20\x3d\x24\x21\x2d\x28\x20\x3c\x2a\x2b\x2a\x28\x20\x3f\x21\x26\x24\x24\x36\x2c\x2e\x20\x29\x27\x20\x24\x31\x39\x20\x2c\x20\x23\x39\x3d\x21\x31\x20\x3c\x2a\x3d\x36\x20\x3c\x36\x3b\x36\x36\x23", 50) != 0){
        puts("Nope");
        exit(1);
    }

    win();
    
}