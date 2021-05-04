#include <constants.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int inner;
    int middle;
    int outer;
    bool always_print;
    bool pressed;
    bool pins[PINS];
} lock;

void print_flag() {
    FILE* flagfile = fopen("flag.txt", "r");
    if (flagfile == NULL) {
        puts("Couldn't find a flag file.");
        return;
    }
    char flag[128];
    fgets(flag, 128, flagfile);
    flag[strcspn(flag, "\n")] = '\x00';
    puts(flag);
}

lock init() {
    lock temp;
    temp.inner = 0;
    temp.middle = 0;
    temp.outer = 0;
    temp.pressed = false;
    temp.always_print = false;
    for (int i = 0; i < PINS; i++) {
        temp.pins[i] = false;
    }
    return temp;
}

void print_lock(lock* l) {
    char outbuf[21][21];
    // for (int i = 0; i < PINS; i++) {
    //     putchar(l->pins[i] ? 'y' : 'n');
    // }
    // putchar('\n');
    // printf("%d-%d-%d\n", l->outer, l->middle, l->inner);

    for (int i = 0; i < OUTBUFSIZE; i++) {
        for (int j = 0; j < OUTBUFSIZE; j++) {
            outbuf[i][j] = ' ';
        }
    }

    for (int i = 0; i < outerlen; i++) {
        outbuf[outerpos[i][0]][outerpos[i][1]] = '.';
    }
    for (int i = 0; i < middlelen; i++) {
        outbuf[middlepos[i][0]][middlepos[i][1]] = '.';
    }
    for (int i = 0; i < innerlen; i++) {
        outbuf[innerpos[i][0]][innerpos[i][1]] = '.';
    }

    outbuf[outerpos[l->outer][0]][outerpos[l->outer][1]] = '#';
    outbuf[middlepos[l->middle][0]][middlepos[l->middle][1]] = '#';
    outbuf[innerpos[l->inner][0]][innerpos[l->inner][1]] = '#';

    if (l->pressed) {
        outbuf[OUTBUFSIZE / 2][OUTBUFSIZE / 2] = '*';
    }

    for (int i = 0; i < OUTBUFSIZE; i++) {
        for (int j = 0; j < OUTBUFSIZE; j++) {
            putchar(outbuf[i][j]);
        }
        putchar('\n');
    }
}

void update_pins(lock* l) {
    if (l->middle == 11 && l->inner == 22) {
        l->pins[4] = l->outer == 7;
    }
    if (l->outer == 7) {
        l->pins[1] = (l->pins[1] && l->pins[4]) ||
                     (l->middle == 8 && l->inner == 9 && l->pins[2] && !l->pressed);
    }
    if (l->outer == 13 && l->middle == 37 && l->inner == 6) {
        if (!l->pressed) {
            l->pins[2] = true;
        }
    } else if (l->pins[1] == l->pressed) {
        l->pins[2] = false;
    }
    if (l->pins[1] && l->pins[4] && !l->pins[2]) {
        l->pins[3] = true;
    }
    l->pins[0] = l->outer == 0 && l->middle == 0 && l->inner == 0;
}

bool process_move(lock* l, char move, int* movecount) {
    (*movecount)++;
    switch (move) {
        case 'i':
            l->inner = (l->inner + 1) % innerlen;
            break;
        case 'I':
            l->inner = (l->inner + innerlen - 1) % innerlen;
            break;
        case 'm':
            l->middle = (l->middle + 1) % middlelen;
            break;
        case 'M':
            l->middle = (l->middle + middlelen - 1) % middlelen;
            break;
        case 'o':
            l->outer = (l->outer + 1) % outerlen;
            break;
        case 'O':
            l->outer = (l->outer + outerlen - 1) % outerlen;
            break;
        case '!':
            l->pressed = !l->pressed;
            break;
        case '?':
            print_lock(l);
            (*movecount)--;
            break;
        case '@':
            l->always_print = !l->always_print;
            (*movecount)--;
            break;
        default:
            return true;
    }
    update_pins(l);
    return false;
}

bool process_input(lock* l, char* moves, int* movecount) {
    while (true) {
        char cur = *moves++;
        if (cur == '\0' || cur == '\n') {
            break;
        }
        if (process_move(l, cur, movecount)) {
            return true;
        }
    }
    return false;
}

bool check_lock(lock* l) {
    for (int i = 0; i < PINS; i++) {
        if (!l->pins[i]) {
            return false;
        }
    }
    return true;
}

int main() {
    setvbuf(stdout, NULL, _IONBF, 0);
    lock l = init();
    int movecount = 0;
    while (true) {
        if (l.always_print) {
            print_lock(&l);
        }
        printf("> ");
        char inp[1024];
        if (!fgets(inp, 1024, stdin)) {
            puts("You walk away from the lock.");
            break;
        }
        if (process_input(&l, inp, &movecount)) {
            puts("Your actions prove to be ineffective against the lock.");
            break;
        }
        if (movecount > 164) {
            puts("Your hands grow weary from the stiffness of the lock.");
            break;
        }
        if (check_lock(&l)) {
            puts("The lock opens.");
            print_flag();
            break;
        }
    }
    return 0;
}
