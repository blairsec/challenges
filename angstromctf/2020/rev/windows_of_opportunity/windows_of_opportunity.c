#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// actf{ok4y_m4yb3_linux_is_s7ill_b3tt3r}

unsigned int analyze(char * str, int i) {
    return (str[i] << 24) + (str[i + 2] << 16) * 2 + (str[i + 4] << 8) + str[i + 6] * 2;
}

unsigned int vals[] = {0x61e87bd6, 0x63cc6f68, 0x74f66bf2, 0x66de34be, 0x7bd679da, 0x6f685f68, 0x6bf26df2, 0x34be34c4, 0x79da7966, 0x5f6862be, 0x6df233d8, 0x34c45fd2, 0x79666cdc, 0x62be69ea, 0x33d86ef0, 0x5fd275be, 0x6cdc78d2, 0x69ea5fe6, 0x6ef069be, 0x75be73e6, 0x78d25f6e, 0x5fe673d2, 0x69be37d8, 0x73e669d8, 0x5f6e6cbe, 0x73d26cc4, 0x37d85f66, 0x69d862e8, 0x6cbe33e8, 0x6cc47466, 0x5f6674e4, 0x62e833fa};
char flag[128];

int main() {
    printf("Welcome to the superior rev challenge compiled for the superior operating system!\n");
    printf("What's the superior flag for this superior rev challenge?\n");
    fgets(flag, 128, stdin);
    int len = strlen(flag) - 1;
    flag[len] = 0;
    if (len != 38) {
        printf("Your flag is way too different from my superior flag!\n");
        exit(1);
    }
    for (int i = 0; i < len - 6; i ++) {
        if (analyze(flag, i) != vals[i]) {
            printf("Maybe if you spent more time using the superior operating system you would actually get the flag.\n");
            exit(1);
        }
    }
    printf("Oh wow a fellow windows user!\n");
}