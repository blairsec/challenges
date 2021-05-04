#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef unsigned char byte;

unsigned int foo(unsigned int a, unsigned int b) {
    return a ^ (b + 1) ^ 1337;
}

unsigned int bar(unsigned int a, unsigned int b, unsigned int c) {
    return a + b * (c + 1);
}

int main(void) {
    setvbuf(stdout, NULL, _IONBF, 0);
    char flag[256];
    FILE* flagfile = fopen("flag.txt", "r");
    if (flagfile == NULL) {
        puts("Couldn't find a flag file.");
        return 1;
    }
    fgets(flag, 256, flagfile);
    fclose(flagfile);
    int flaglen = strcspn(flag, "\n");
    flag[flaglen] = '\0';
    for (unsigned int i = 0; i < flaglen; i++) {
        flag[i] ^= (byte)(i * 17);
    }
    srand(time(NULL));
    puts("Welcome to the infinity gauntlet!");
    puts("If you complete the gauntlet, you'll get the flag!");
    int round = 1;
    while (true) {
        printf("=== ROUND %d ===\n", round);
        int pos = rand() % flaglen;
        byte posval = pos + round;
        unsigned int encval;
        if (round < 50) {
            encval = rand() % 65536;
        } else {
            encval = (posval << 8) | (byte) flag[pos];
        }
        if (rand() % 2) {
            int arg = rand() % 3;
            if (arg == 0) {
                unsigned int arg2 = rand() % 1337;
                unsigned int arg3 = foo(encval, arg2);
                printf("foo(?, %u) = %u\n", arg2, arg3);
            } else if (arg == 1) {
                unsigned int arg1 = rand() % 1337;
                unsigned int arg3 = foo(arg1, encval);
                printf("foo(%u, ?) = %u\n", arg1, arg3);
            } else {
                unsigned int arg2 = rand() % 1337;
                unsigned int arg1 = encval ^ 1337 ^ (arg2 + 1);
                printf("foo(%u, %u) = ?\n", arg1, arg2);
            }
        } else {
            int arg = rand() % 4;
            if (arg == 0) {
                unsigned int arg2 = rand() % 1337;
                unsigned int arg3 = rand() % 1337;
                unsigned int arg4 = bar(encval, arg2, arg3);
                printf("bar(?, %u, %u) = %u\n", arg2, arg3, arg4);
            } else if (arg == 1) {
                unsigned int arg1 = rand() % 1337;
                unsigned int arg3 = rand() % 1337;
                unsigned int arg4 = bar(arg1, encval, arg3);
                printf("bar(%u, ?, %u) = %u\n", arg1, arg3, arg4);
            } else if (arg == 2) {
                unsigned int arg1 = rand() % 1337;
                unsigned int arg2 = rand() % 1337;
                unsigned int arg4 = bar(arg1, arg2, encval);
                printf("bar(%u, %u, ?) = %u\n", arg1, arg2, arg4);
            } else {
                unsigned int arg2 = encval > 1337 ? rand() % 1337 : rand() % encval;
                unsigned int arg3 = encval / arg2 - 1;
                unsigned int arg1 = encval % arg2;
                printf("bar(%u, %u, %u) = ?\n", arg1, arg2, arg3);
            }
        }
        unsigned int inp;
        scanf("%u", &inp);
        if (inp != encval) {
            puts("Wrong!");
            return 0;
        } else {
            round++;
            printf("Correct! Maybe round %d will get you the flag ;)\n", round);
        }
    }
    return 0;
}
