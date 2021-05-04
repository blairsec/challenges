#include <stdio.h>
#include <sys/ptrace.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <signal.h>

unsigned int serial = 0xf00dbabe;
// unsigned int serial = 0x1337beef;
int status = 0xff;

unsigned int reg;

union {
    int ints[7];
    char chars[28];
} vals;

int something[] = {0x5cd883ea, 0xef6680c0, 0xafcc7086, 0x7e3ab87f, 0x7dc5773a, 0x38208035, 0x0456c3f5};

int gb(int n, int b) {
    return (n >> b) & 1;
}

unsigned int gen() {
    unsigned int n = gb(reg, 0) ^ gb(reg, 2) ^ gb(reg, 3) ^ gb(reg, 5);
    reg >>= 1;
    reg |= n << (sizeof(reg) * 8 - 1);
    return reg;
}

unsigned int garble(unsigned int a, unsigned int b) {
    return a + b;
}

int main() {
    setvbuf(stdout, NULL, _IONBF, 0);
    status = ptrace(PTRACE_TRACEME, 1337, 0x1337, 1337);
    if (status == -1 || serial != 0x1337beef) {
        printf("Hey you're not supposed to get the flag! Freeze!\n");
        signal(SIGINT, SIG_IGN);
        signal(SIGTERM, SIG_IGN);
        signal(SIGABRT, SIG_IGN);
        while (1) {}
    }
    reg = serial;
    for (int i = 0; i < 7; i ++) {
        vals.ints[i] = garble(something[i] ^ status, gen());
    }
    printf("Here have a flag:\n%s\n", vals.chars);
}
