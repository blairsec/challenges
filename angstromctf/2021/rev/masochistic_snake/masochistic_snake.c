#define _GNU_SOURCE

#include <fcntl.h>
#include <ncurses.h>
#include <signal.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <sys/errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#define EMPTY 0
#define SNAKE 1
#define FOOD 2
#define POISON 3

#define RIGHT 0
#define UP 1
#define LEFT 2
#define DOWN 3

#define QUIT_RESULT 0
#define WIN_RESULT 1
#define LOSE_RESULT 2

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

typedef unsigned char byte;

byte lfsrBits[64] = {46, 10, 1, 12, 0, 55, 48, 18, 17, 32, 56, 59, 16, 35, 26, 38, 36, 4, 8, 63, 9, 15, 20, 6, 54, 31, 23, 25, 2, 27, 42, 11, 52, 30, 19, 61, 62, 29, 33, 34, 58, 50, 14, 51, 21, 49, 43, 47, 57, 13, 3, 24, 41, 37, 7, 44, 5, 53, 28, 40, 60, 39, 22, 45};

unsigned long long advanceLfsr(unsigned long long n) {
    unsigned long long bit = (n & 1) ^ ((n >> 1) & 1) ^ ((n >> 3) & 1) ^ ((n >> 4) & 1) ^ 1;
    return (bit << 63) | (n >> 1);
}

byte randPos(byte* board) {
    byte pos = rand();
    while (board[pos] != EMPTY) {
        pos++;
    }
    return pos;
}

int msleep(int msec) {
    struct timespec ts;
    int res;

    if (msec < 0) {
        errno = EINVAL;
        return -1;
    }

    ts.tv_sec = msec / 1000;
    ts.tv_nsec = (msec % 1000) * 1000000;

    do {
        res = nanosleep(&ts, &ts);
    } while (res && errno == EINTR);

    return res;
}

int doSnake(int ms) {
    byte board[256] = {SNAKE};
    byte snake[256] = {0};
    byte head = 1;
    byte tail = 0;
    byte dir = RIGHT;
    timeout(0);
    board[randPos(board)] = FOOD;
    int poisonPos[50];
    for (int i = 0; i < 50; i++) {
        poisonPos[i] = randPos(board);
        board[poisonPos[i]] = POISON;
    }
    mvprintw(0, 0, "Here's a cheerleader! ...what, did you expect to see a snake?");
    mvprintw(1, 0, "\\o/");
    unsigned long long lfsr = 0xa1025d352cc2655fULL;
    unsigned int counter = 0;
    while (true) {
        if (counter % 2) {
            mvaddch(1, 0, '/');
            mvaddch(1, 2, '\\');
        } else {
            mvaddch(1, 0, '\\');
            mvaddch(1, 2, '/');
        }
        refresh();
        int inp = ERR;
        while (true) {
            int curinp = getch();
            if (curinp == ERR) {
                break;
            } else {
                inp = curinp;
            }
        }
        unsigned int bitOffset = (counter % 32) * 2;
        unsigned int moveOffset = (((lfsr >> lfsrBits[bitOffset]) & 1) << 1) | ((lfsr >> lfsrBits[bitOffset + 1]) & 1);
        if (inp != ERR) {
            switch (inp) {
                case 'q':
                    return QUIT_RESULT;
                case 'w':
                    dir = (UP + moveOffset) % 4;
                    break;
                case 'a':
                    dir = (LEFT + moveOffset) % 4;
                    break;
                case 's':
                    dir = (DOWN + moveOffset) % 4;
                    break;
                case 'd':
                    dir = (RIGHT + moveOffset) % 4;
                    break;
            }
        }
        byte pos = snake[(byte)(head - 1)];
        byte newpos;
        switch (dir) {
            case UP:
                if (pos / 16 < 1) {
                    return LOSE_RESULT;
                }
                newpos = pos - 16;
                break;
            case DOWN:
                if (pos / 16 > 14) {
                    return LOSE_RESULT;
                }
                newpos = pos + 16;
                break;
            case LEFT:
                if (pos % 16 < 1) {
                    return LOSE_RESULT;
                }
                newpos = pos - 1;
                break;
            case RIGHT:
                if (pos % 16 > 14) {
                    return LOSE_RESULT;
                }
                newpos = pos + 1;
                break;
        }
        switch (board[newpos]) {
            case EMPTY:
                board[snake[tail]] = EMPTY;
                tail++;
                break;
            case SNAKE:
            case POISON:
                return LOSE_RESULT;
            case FOOD:
                for (int i = 0; i < 50; i++) {
                    board[poisonPos[i]] = EMPTY;
                }
                board[randPos(board)] = FOOD;
                for (int i = 0; i < 50; i++) {
                    poisonPos[i] = randPos(board);
                    board[poisonPos[i]] = POISON;
                }
                break;
        }
        board[newpos] = SNAKE;
        snake[head] = newpos;
        head++;
        int diff;
        if (head > tail) {
            diff = head - tail;
        } else {
            diff = tail + 256 - head - 1;
        }
        if (diff >= 25) {
            return WIN_RESULT;
        }
        // for (int i = 0; i < 256; i++) {
        //     mvaddch(i / 16, i % 16, " XO#"[board[i]]);
        // }
        // for (int i = 0; i < 17; i++) {
        //     mvaddch(i, 16, '#');
        //     mvaddch(16, i, '#');
        // }
        // refresh();
        msleep(ms);
        counter++;
    }
}

int main(int argc, char** argv) {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);
    printw("Press any key to continue...");
    getch();
    erase();
    int delay = 0;
    if (argc >= 2) {
        delay = atoi(argv[1]);
    }
    if (delay <= 0) {
        delay = 500;
    }
    int result = doSnake(delay);
    delwin(stdscr);
    endwin();
    refresh();
    switch (result) {
        case QUIT_RESULT:
            puts("You quit >:(((");
            break;
        case WIN_RESULT:
            puts("You won :)))");
            print_flag();
            break;
        case LOSE_RESULT:
            puts("You lost >:)))");
            break;
    }
    return 0;
}
