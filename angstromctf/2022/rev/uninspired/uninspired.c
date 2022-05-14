#include <stdio.h>
#include <string.h>
#include <stdint.h>

void print_flag(char* inp) {
    uint8_t schedule[256];
    for (int i = 0; i < 256; i ++) {
        schedule[i] = i;
    }
    int j = 0;
    for (int i = 0; i < 256; i ++) {
        j = (j + schedule[i] + inp[i % 10]) % 256;
        uint8_t tmp = schedule[i];
        schedule[i] = schedule[j];
        schedule[j] = tmp;
    }
    char flag[43] = "\xef\x75\xfd\x9d\x81\x9c\x10\x99\x45\x6e\x44\x43\x05\x39\x9c\xb7\xb2\xec\xae\x17\x3c\x71\x3a\x83\x45\xbf\x47\x6d\x10\x3d\x4a\x6d\xdd\x0f\x73\x6a\x5c\xf7\x34\xe4\xef\xa6\xd5";
    j = 0;
    for (int i = 0; i < 43; i ++) {
        j = (j + schedule[i]) % 256;
        uint8_t tmp = schedule[i];
        schedule[i] = schedule[j];
        schedule[j] = tmp;
        putchar(flag[i] ^ schedule[(schedule[i] + schedule[j]) % 256]);
    }
    putchar('\n');
}

int main(void) {
    puts("there's no more inspiration :(");
    char inp[16];
    fgets(inp, 16, stdin);
    int inplen = strcspn(inp, "\n");
    inp[inplen] = '\0';
    if (inplen != 10) {
        puts("that's not very inspiring :(");
        return 1;
    }
    int counts[10] = {0};
    for (int i = 0; i < 10; i ++) {
        if (inp[i] < '0' || inp[i] > '9') {
            puts("I don't like your inspiration :(");
            return 1;
        }
        counts[inp[i] - '0'] ++;
    }
    for (int i = 0; i < 10; i ++) {
        if (counts[i] != inp[i] - '0') {
            puts("that's not good inspiration :(");
            return 1;
        }
    }
    puts("yay I'm inspired now, have a flag :)");
    print_flag(inp);
    return 0;
}
