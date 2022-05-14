#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NSTACKS 100
#define STACK_SIZE 4096
#define SCRATCH_SIZE 1024
#define STACKO(ty, offset) ((ty*)(stacks[sp] + (offset)))
#define SSTACKO(ty, offset) (((ty*)stacks[sp]) + (offset))
#define STACK(ty, o) (*STACKO(ty, o))
#define SSTACK(ty, o) (*SSTACKO(ty, o))
#define TERNARY(cond, t, f) ((cond) * ((t) - (f)) + (f))

// variables
#define MAIN_CUR STACK(uint64_t, 0)
#define MAIN_I STACK(uint64_t, 8)
#define MAIN_SEEN STACKO(uint8_t, 16)
#define LOOKUP_I STACK(uint64_t, 0)
#define INP_C STACK(uint64_t, 0)
#define INP_SIP STACK(uint64_t, 8)

enum block {
    main_start,
    main_init,
    main_read_end,
    main_end,
    main_read_start,
    main_read_actual,
    main_success,
    lookup_start,
    lookup_loop_start,
    lookup_failure,
    lookup_loop_actual2,
    lookup_loop_actual,
    lookup_success,
    inp_start,
    inp_verify,
    inp_verify2
};

int main(void) {
    enum block ip = main_start;
    uint8_t stacks[NSTACKS][STACK_SIZE];
    uint64_t scratch[SCRATCH_SIZE];
    int sp = 0;
    char graph[] = "NfTRcD1ontrw}4{mFl_Ad0ua";
    int edges[] = {20, 10, 0, 22, 14, 4, 5, 22, 15, 15, 18, 7, 10, 2, 19, 19, 9, 13, 8, 17, 11, 12, 0, 4};
    int edges2[] = {17, 14, 15, 6, 9, 12, 5, 10, 18, 1, 7, 21, 16, 14, 16, 2, 17, 0, 10, 8, 22, 3, 2, 19};
    while (1) {
        switch (ip) {
            case main_start:
                puts("I call our world Flatland, not because we call it so, but to make its nature clearer to you, my happy solvers, who are privileged to have control flow.");
                memset(MAIN_SEEN, 0, 256);
                scratch[0] = main_init;
                ip = inp_start;
                sp++;
                break;
            case main_init:
                MAIN_CUR = scratch[1];
                MAIN_SEEN[MAIN_CUR] = 1;
                MAIN_I = 1;
                ip = main_read_start;
                break;
            case main_read_start:
                scratch[0] = main_read_actual;
                ip = inp_start;
                sp++;
                break;
            case main_read_actual:
                ip = TERNARY((edges[MAIN_CUR] == scratch[1] || edges[scratch[1]] == MAIN_CUR || edges2[MAIN_CUR] == scratch[1] || edges2[scratch[1]] == MAIN_CUR) && MAIN_SEEN[scratch[1]] == 0, main_read_end, main_end);
                break;
            case main_read_end:
                MAIN_CUR = scratch[1];
                MAIN_SEEN[MAIN_CUR] = 1;
                MAIN_I++;
                ip = TERNARY(graph[MAIN_I] == 0, main_success, main_read_start);
                break;
            case main_success:
                puts("Now you have truly understood the secrets of Flatland.");
                return 0;
            case main_end:
                puts("All the substantial binaries of Flatland itself appear no better than the offspring of a diseased imagination, or the baseless instructions of a CPU.");
                return 1;
            case lookup_start:
                LOOKUP_I = 0;
                ip = lookup_loop_start;
                break;
            case lookup_loop_start:
                ip = TERNARY(graph[LOOKUP_I] != 0, lookup_loop_actual, lookup_failure);
                break;
            case lookup_loop_actual:
                ip = TERNARY(graph[LOOKUP_I] == scratch[1], lookup_success, lookup_loop_actual2);
                break;
            case lookup_loop_actual2:
                LOOKUP_I++;
                ip = lookup_loop_start;
                break;
            case lookup_failure:
                scratch[1] = -1;
                ip = scratch[0];
                sp--;
                break;
            case lookup_success:
                scratch[1] = LOOKUP_I;
                ip = scratch[0];
                sp--;
                break;
            case inp_start:
                INP_C = getc(stdin);
                ip = TERNARY(INP_C != EOF, inp_verify, main_end);
                break;
            case inp_verify:
                INP_SIP = scratch[0];
                scratch[0] = inp_verify2;
                scratch[1] = INP_C;
                ip = lookup_start;
                sp++;
                break;
            case inp_verify2:
                ip = TERNARY(scratch[1] != -1, INP_SIP, main_end);
                sp--;
                break;
        }
    }
}
