#include <encstrings.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPLEN 256
#define INP                           \
    if (!fgets(inp, INPLEN, stdin)) { \
        break;                        \
    }                                 \
    inp[strcspn(inp, "\n")] = '\0'

char* decString(int i) {
    int start = offsets[i];
    int end = offsets[i + 1];
    int diff = end - start;
    char* ret = malloc(diff + 1);
    ret[diff] = '\0';
    byte seed = i;
    for (int j = 0; j < diff; j++) {
        ret[j] = encstrings[start + j] ^ seed;
        seed = seed * ret[j] + 17 * i;
    }
    return ret;
}

void outString(int i) {
    char* str = decString(i);
    puts(str);
    free(str);
}

bool cmpString(char* str, int i) {
    char* ostr = decString(i);
    bool ret = !strcmp(str, ostr);
    free(ostr);
    return ret;
}

int main(void) {
    setvbuf(stdout, NULL, _IONBF, 0);
    outString(1);
    char inp[256];
    int state = 0;
    bool isThereKmh = true;
    bool hasSnake = false;
    while (true) {
        outString(0);
        INP;
        if (state == 0) {
            if (cmpString(inp, 2)) {
                if (isThereKmh) {
                    outString(3);
                } else {
                    outString(8);
                }
            } else if (cmpString(inp, 6)) {
                outString(7);
                break;
            } else if (cmpString(inp, 9)) {
                outString(10);
            } else if (cmpString(inp, 11)) {
                if (isThereKmh) {
                    outString(12);
                    break;
                } else {
                    outString(13);
                    state = 1;
                }
            } else if (cmpString(inp, 15) && !hasSnake) {
                hasSnake = true;
                outString(16);
            } else if (cmpString(inp, 14) && hasSnake) {
                outString(17);
                isThereKmh = false;
            } else {
                outString(4);
            }
        } else {
            if (cmpString(inp, 2)) {
                outString(18);
            } else if (state == 1337 && cmpString(inp, 23)) {
                outString(24);
                char* filename = decString(25);
                char* mode = decString(26);
                FILE* flagfile = fopen(filename, mode);
                free(filename);
                free(mode);
                if (flagfile == NULL) {
                    outString(27);
                } else {
                    char flag[256];
                    fgets(flag, 256, flagfile);
                    flag[strcspn(flag, "\n")] = '\0';
                    puts(flag);
                }
                outString(28);
                return 0;
            } else if (cmpString(inp, 19)) {
                state *= 2;
                outString(21);
            } else if (cmpString(inp, 20)) {
                state = state * 2 + 1;
                outString(22);
            } else {
                outString(4);
            }
        }
    }
    outString(5);
    return 0;
}
