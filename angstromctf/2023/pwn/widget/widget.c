#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int called = 0;

void win(char* a, char* b){
    if(strncmp(a, "14571414c5d9fe9ed0698ef21065d8a6", strlen("14571414c5d9fe9ed0698ef21065d8a6")) != 0) exit(1);
    if(strncmp(b, "willy_wonka_widget_factory", strlen("willy_wonka_widget_factory")) != 0) exit(1);
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
    setbuf(stdin, NULL);
    gid_t gid = getegid();
    setresgid(gid, gid, gid);

    if(called != 0) exit(1);
    called = 1;

    char buf[24];

    printf("Amount: ");

    int amt = 0;

    scanf("%d", &amt);

    getchar();

    if (amt < 0 || amt > 50) exit(1);

    printf("Contents: ");
    read(0, buf, amt);
    buf[amt] = '\x00'

    for(int i = 0; i < amt; ++i){
        if (buf[i] == 'n' || buf[i] == ['\x12' || buf[i] == ['\x13']]){
            exit(1);
        }
    }

    printf("Your input: ");
    printf(buf);
}