#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <setjmp.h>
#include <sys/time.h>

#define SIGS 7
#define FLAGSIZE 64
#define uch unsigned char

jmp_buf jbuf;

void print_flag() {
        FILE * file = fopen("flag.txt", "r");
        char flag [FLAGSIZE];
        if (file == NULL) {
                printf("Cannot read flag file.\n");
                exit(1);
        }
        fgets(flag, FLAGSIZE, file);
        printf("%s", flag);
}

struct itimerval tim;

int * test = NULL;

uch cursor = 0;
uch base = 'F';
uch stack[256];
uch slen = 0;
uch stack2[256];
uch slen2 = 0;
uch reg = 0;
uch reg2 = 0;
uch inst;

// Ga^AHkI4<:
//  !"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_`abcdefghijklmnopqrstuvwxyz{|}~
uch input[128];

int ind = 0;
uch ins[] = {
    0x62, 0xf0, 0xf7, 0xfc, // read; push; push2
    0xcc, 0x4, 0xf6, 0x86, // cursor += 4
    0xf0, 0xf7, 0xfc, 0x69, // read; push; push2
    0xcc, 0x2, 0x87, 0x1b, // cursor += 2
    0xcc, 0x0, 0xf8, 0xfb, // reg = 0; pop reg2
    0xf0, 0xf1, 0xfb, // read; add; swap
    0xf8, 0xf1, 0x69, // pop; add
    0xcc, 0x6, 0xf9, // assert reg == 6
    0xfd, 0xf5, // pop2 reg2
    0xf0, 0xf3, 0xf5, // reg2 *= read
    0xfd, 0xf3, 0x69, // pop2; reg *= reg2
    0xcc, 0x6, 0xf9, 0x69, // assert reg == 6,
    0xcc, 0x100-0x5, 0xf6, 0x8e, 0x76, // cursor -= 5
    0xf0, 0xcc, 0x1b, 0xf9, // read; assert reg == 0x1b
    0x77, 0x69, // base
    0xcc, 0x1, 0xf6, 0x86, // cursor += 1
    0x5c, 0xf0, 0xcc, 0x100-0x3, 0xf9, // read; assert reg == -0x3
    0x69, 0xf7, 0xcc, 0x3, 0xf6, 0x86, // push; cursor += 3
    0x4f, 0xf0, 0xcc, 0, 0xfb, // reg = 0; read reg2
    0xf8, 0xf1, 0xf7, // pop; reg += reg2; push
    0x69, 0xcc, 0x2, 0xf6, 0x8e, // cursor += 2
    0x1b, 0xf0, 0xcc, 0x100-0x2e, 0xf9, // read; assert reg == -0x2e
    0x6a, 0x69, // base
    0xcc, 0x100-0x4, 0xf6, 0x87, // cursor -= 4,
    0x69, 0xf0, 0xfc, 0xcc, 0xe, 0xf9, // read; push2; assert reg == 0xe
    0x4f, 0xcc, 0x5, 0xf6, 0x86, // cursor += 5
    0xf0, 0xf5, 0xfd, 0xf4, // read reg2; pop2; reg ^= reg2
    0xf5, 0xf8, 0xf9, 0xf7, // reg2 = reg; pop; assert reg == reg2; push
    0x76, 0xcc, 0x1, 0xf6, 0x86, // cursor += 1
    0xf0, 0xf5, 0xf7, 0xf9, // read reg2; pop; assert reg == reg2
    0x5c // flag
};

uch divzero[] = "\x48\x31\xc0\x48\xf7\xf0";

void runCode(uch * code) {
    int (*func)() = (int(*)()) code;
    func();
}

uch getValue() {
    return input[cursor] - base;
}

/*void debug() {
    if (inst == 0x1) {
        printf("inst=[%02x] cursor=[%02x] value=[%02x] reg=[%02x] reg2=[%02x] stack=[", inst, cursor, getValue(), reg, reg2);
        for (int i = 0; i < slen; i ++) {
            printf("%02x", stack[i]);
            if (i < slen - 1) {
                printf(" ");
            }
        }
        printf("] stack2=[");
        for (int i = 0; i < slen2; i ++) {
            printf("%02x", stack2[i]);
            if (i < slen2 - 1) {
                printf(" ");
            }
        }
        printf("]\n");
    }
}*/

// SIGABRT
void hand1() {
    puts("A camera emerges and it eyes you down.");
    puts("Suddenly, the trap shut down.");
    puts("You make a sprint for the diminishing light.");
    puts("And you see a man, much to your delight.");
    puts("He whispered, \"look inside you to find the key.\"");
    printf("And you told him that ");
    print_flag();
    exit(0);
}

// SIGALRM
void hand2() {
    setjmp(jbuf);
    inst = ins[ind++];
    // debug();
    if (inst == 0x62) {
        (*test)++;
    } else if (inst == 0xcc) {
        uch yes = inst;
        inst = ins[ind++];
        runCode(&yes);
    } else if (inst == 0x5c) {
        abort();
    } else if ((inst >> 4) == 8) {
        uch yes = inst & 0xf;
        runCode(&yes);
    } else if ((inst >> 4) == 0xf) {
        raise(SIGINT);
    } else if (inst % 13 == 1) {
        return;
    } else if (inst % 13 == 2) {
        runCode(divzero);
    }
    hand2();
}

// SIGILL
void hand3() {
    cursor += reg;
    longjmp(jbuf, 0);
}

// SIGFPE
void hand4() {
    base = input[cursor];
    longjmp(jbuf, 0);
}

// SIGSEGV
void hand5() {
    puts("The first and last trap purred and stirred.");
    puts("It declared, \"What is the password?\"");
    fgets(input, 128, stdin);
    int len = strlen(input);
    if (input[len - 1] == '\n') {
        input[len - 1] = 0;
    }
    if (strlen(input) != 10) {
        puts("Your password was wrong and the trap unshaken.");
        puts("Try again, when it reawakens.");
        exit(1);
    }
    longjmp(jbuf, 1);
}

// SIGTRAP
void hand6() {
    reg2 = inst;
    longjmp(jbuf, 0);
}

// SIGINT
void hand7() {
    int op = inst - 0xf0;
    uch temp;
    switch(op) {
        case 0:
            reg = getValue();
            break;
        case 1:
            reg += reg2;
            break;
        case 2:
            reg -= reg2;
            break;
        case 3:
            reg *= reg2;
            break;
        case 4:
            reg ^= reg2;
            break;
        case 5:
            reg2 = reg;
            break;
        case 6:
            reg = reg2;
            break;
        case 7:
            stack[slen++] = reg;
            break;
        case 8:
            reg = stack[--slen];
            break;
        case 9:
            if (reg != reg2) {
                puts("The trap is not approving of your trip.");
                puts("It blocks your passage with its whip.");
                exit(1);
            }
            break;
        case 10:
            reg = input[cursor];
            break;
        case 11:
            temp = reg;
            reg = reg2;
            reg2 = temp;
            break;
        case 12:
            stack2[slen2++] = reg;
            break;
        case 13:
            reg = stack2[--slen2];
            break;
        default:
            puts("The trap shorts out and sparks like crazy.");
            puts("But a spark hits you and you become hazy.");
            exit(1);
    }
    return;
}

void (*handlers[SIGS])() = {hand1, hand2, hand3, hand4, hand5, hand6, hand7};
int sigs[SIGS] = {SIGABRT, SIGALRM, SIGILL, SIGFPE, SIGSEGV, SIGTRAP, SIGINT};

int findsig(int signum) {
    for (int i = 0; i < SIGS; i ++) {
        if (signum == sigs[i]) {
            return i;
        }
    }
    return -1;
}

void sighandle(int signum) {
    (*handlers[findsig(signum)])();
}

int main() {
    setvbuf(stdout, NULL, _IONBF, 0);

	gid_t gid = getegid();
	setresgid(gid, gid, gid);
	
	tim.it_value.tv_sec = 0;
	tim.it_value.tv_usec = 1000;
	tim.it_interval.tv_sec = 0;
	tim.it_interval.tv_usec = 1000;
	
    puts("It's a rainy night and you're lost in the forest.");
    puts("Your house is nowhere to be seen and you're quite discouraged.");
    puts("But alas, in the distance, a fading beacon!");
    puts("You fill with hope, but it'll soon weaken.");
    puts("But can you make it before you collapse?");
    puts("Through thick and thin, and past the traps.");
    for (int i = 0; i < SIGS; i ++) {
        if (signal(sigs[i], sighandle) == SIG_IGN) {
            puts("Something feels off, you can't go longer.");
            puts("A bear emerges, and he is stronger.");
            exit(1);
        }
    }
    setitimer(ITIMER_REAL, &tim, NULL);
    while (1) {
        getchar();
    }
}