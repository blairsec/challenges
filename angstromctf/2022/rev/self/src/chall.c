#include<stdio.h>
#include<stdint.h>

#define u32 uint32_t
#define u16 uint16_t
#define u8 uint8_t

#define PROG_START 0x800


extern u32 mem[];

typedef enum {
    O_HLT = 0x0,
    O_NOP = 0xff,
    O_DAT = 0xda,
    O_MOV = 0xd6, // used
    O_IMM = 0xd8, // used
    O_JMP = 0xa6, // used
    O_JIZ = 0x69, // used
    O_ADD = 0x16, // used
    O_SUB = 0x17,
    O_XOR = 0x18,
    O_OUT = 0xf6, // used
    O_INP = 0xf7, // used
#ifdef DEBUG
    D_PRT = 0x01,
#endif
} __attribute__((packed)) OPCODES;

int main() {
    u32 pc = PROG_START;
    do {
        switch (mem[pc] >> 24) {

            case O_MOV: 
                {
                    // aabbbccc
                    // mem[bbb] = mem[ccc]
                    mem[(mem[pc] >> 12) & 0xfff] = mem[mem[pc] & 0xfff];
                    break;
                }
#ifdef DEBUG
            case D_PRT:
                {
                    u32 str = mem[pc] & 0x00ffffff;
                    fprintf(stderr, "0x%04x: [ %06x %06x %06x ]  %6s \n", pc, mem[0x858], mem[0x858 + 1], mem[0x858 + 2], (char *)&str);
                    fprintf(stderr, "[ %06x %06x %06x %06x %06x ] \n", mem[0x1ff], mem[0x1ff + 1], mem[0x1ff + 2], mem[0x1ff + 3], mem[0x1ff + 4]);
                    fprintf(stderr, "           (%06x %06x %06x %06x)\n", mem[0x10 + 0], mem[0x10 + 1], mem[0x10 + 2], mem[0x10 + 3]);
                    fprintf(stderr, "           (%06x %06x %06x %06x)\n", mem[0x10 + 4], mem[0x10 + 5], mem[0x10 + 6], mem[0x10 + 7]);
                    fprintf(stderr, "           (%06x %06x %06x %06x)\n", mem[0x10 + 8], mem[0x10 + 9], mem[0x10 + 10], mem[0x10 + 11]);
                    fprintf(stderr, "           (%06x %06x %06x %06x)\n", mem[0x10 + 12], mem[0x10 + 13], mem[0x10 + 14], mem[0x10 + 15]);
                    fprintf(stderr, "\n");
                    break;
                }
#endif


            case O_IMM:
                {
                    // aabbbccc
                    // mem[bbb] = ++000ccc
                    mem[(mem[pc] >> 12) & 0xfff] = (mem[(mem[pc] >> 12) & 0xfff] & 0xff000000) | (mem[pc] & 0xfff);
                    break;
                }

            case O_JMP:
                {
                    // aabbbccc
                    // pc = bbb
                    pc = (mem[pc] >> 12) & 0xfff;
                    break;
                }

            case O_JIZ:
                {
                    // aabbbcc
                    // if mem[bbb] == 0:
                    //      pc += ccc
                    pc += mem[(mem[pc] >> 12) & 0xfff] == 0 ? mem[pc] & 0xfff : 1;
                    pc -= 1;
                    pc &= 0xfff;
                    break;
                }

            case O_ADD:
                {
                    // aabbbccc
                    // mem[bbb] = mem[bbb] + mem[ccc]
                    u32 ind = mem[(mem[pc] >> 12) & 0xfff] & 0xffffff;
                    ind += mem[mem[pc] & 0xfff];
                    mem[(mem[pc] >> 12) & 0xfff] &= 0xff000000;
                    mem[(mem[pc] >> 12) & 0xfff] += (ind & 0xffffff);
                    break;
                }

            case O_SUB:
                {
                    // aabbbccc
                    // mem[bbb] = mem[bbb] - mem[ccc]
                    u32 ind = mem[(mem[pc] >> 12) & 0xfff] & 0xffffff;
                    ind -= mem[mem[pc] & 0xfff];
                    mem[(mem[pc] >> 12) & 0xfff] &= 0xff000000;
                    mem[(mem[pc] >> 12) & 0xfff] += (ind & 0xffffff);
                    break;
                }
            case O_XOR:
                {
                    // aabbbccc
                    // mem[bbb] = mem[bbb] ^ mem[ccc]
                    mem[(mem[pc] >> 12) & 0xfff] ^= mem[mem[pc] & 0xfff];
                    break;
                }


            case O_INP:
                {
                    // aabbbccc
                    // mem[bbb] = input()
                    mem[(mem[pc] >> 12) & 0xfff] = (mem[(mem[pc] >> 12) & 0xfff] & 0xff000000) | getchar();
                    break;
                }

            case O_OUT:
                {
                    // aabbbccc
                    // print(mem[bbb])
                    putchar(mem[(mem[pc] >> 12) & 0xfff] & 0xffffff);
                    break;
                }
            case O_NOP:
                {
                    break;
                }
            default:
                {
                    return 0;
                }

        }
        pc++;
    } while (mem[pc] != O_HLT);
}
