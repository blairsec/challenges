#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char name[50] = {};

void flag(int a, char * b){
    if (a == 0x1337){
        if (strncmp(b, "bobby", 5) == 0){
            char flag[128];

            FILE *file = fopen("flag.txt","r");
            if (!file) {
                puts("Error: missing flag.txt.");
                exit(1);
            }

            fgets(flag, 128, file);
            puts(flag);
        }
    }
}

void vuln(){
    char address[50];
    printf("Name: ");
    scanf("%49s", name);
    getchar();

    printf("Address: ");
    gets(address);
}

int main() {
    setbuf(stdout, NULL);
    vuln();

    return 0;
}