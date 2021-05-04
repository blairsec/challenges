#define _GNU_SOURCE

#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

#define PEQ(A, B, C, D) \
    (((A) == (C) && (B) == (D)) || ((A) == (D) && (B) == (C)))
#define PEQS(A, B, C, D) ((A) == (C) && (B) == (D))
#define ABS(A) (((A) < 0) ? -(A) : (A))
#define MIN(A, B) (((A) < (B)) ? (A) : (B))
#define MAX(A, B) (((A) > (B)) ? (A) : (B))
#define BOUND(A, L, H) MAX(MIN(A, H), L)
#define ITER(A, B, C) for (int C = MIN(A, B); C < MAX(A, B); C++)
#define RETI(A, B, C) for (int C = MAX(A, B); C > MIN(A, B); C--)
#define ITEREQ(A, B, C) for (int C = MIN(A, B); C <= MAX(A, B); C++)
#define RETIEQ(A, B, C) for (int C = MAX(A, B); C >= MIN(A, B); C--)
#define ITERNEQ(A, B, C) for (int C = MIN(A, B) + 1; C < MAX(A, B); C++)
#define RETINEQ(A, B, C) for (int C = MAX(A, B) - 1; C > MIN(A, B); C--)
#define DIR(A, B) (((A) == (B)) ? 0 : ((A) < (B)) ? 1 : -1)
#define GOTO(A, B, C) for (int C = (A); C != (B); C += DIR(A, B))
#define GOTOEQ(A, B, C) for (int C = (A); C != (B) + DIR(A, B); C += DIR(A, B))
#define GOTONEQ(A, B, C) for (int C = (A) + DIR(A, B); C != (B); C += DIR(A, B))
#define ATER(A, B, C, D, E, F) \
    (((A) && (B)) ? (F) : (A) ? (D) : (B) ? (E) : (C))
#define NATER(A, B, C, D, E, F) ATER(!(A), !(B), C, D, E, F)

#define CORNER '+'
#define VERT '|'
#define HORIZ '-'
#define BLANK ' '
#define FLAGSIZE 128

void print_flag() {
    gid_t gid = getegid();
    setresgid(gid, gid, gid);
    FILE *file = fopen("flag.txt", "r");
    char flag[FLAGSIZE];
    if (file == NULL) {
        printf("Cannot read flag file.\n");
        exit(1);
    }
    fgets(flag, FLAGSIZE, file);
    printf("%s", flag);
}

int cur_r = 0;
int cur_c = 0;

int board[9][9];
int seen[10];

int is_number(char c) { return (c >= '0') && (c <= '9'); }

int char_to_num(char c) { return c - '0'; }

char num_to_char(int n) { return n + '0'; }

char val_to_disp(int n) {
    if (n == 0) {
        return BLANK;
    }
    return num_to_char(n);
}

void update_cursor(int nr, int nc) {
    attron(COLOR_PAIR(2));
    mvaddch(cur_r * 2 + 1, cur_c * 4 + 2, val_to_disp(board[cur_r][cur_c]));
    attroff(COLOR_PAIR(2));
    cur_r = (nr + 9) % 9;
    cur_c = (nc + 9) % 9;
    attron(COLOR_PAIR(3));
    mvaddch(cur_r * 2 + 1, cur_c * 4 + 2, val_to_disp(board[cur_r][cur_c]));
    attroff(COLOR_PAIR(3));
}

void advance_cursor() {
    int nr = cur_r;
    int nc = cur_c + 1;
    if (nc >= 9) {
        nc %= 9;
        nr ++;
        nr %= 9;
    }
    update_cursor(nr, nc);
}

void update_board() {
    attron(COLOR_PAIR(2));
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            mvaddch(i * 2 + 1, j * 4 + 2, val_to_disp(board[i][j]));
        }
    }
    attroff(COLOR_PAIR(2));
    update_cursor(cur_r, cur_c);
}

int gen_value(int r, int c, int v) {
    int k = r * 100 + c * 10 + v;
    k = ((k ^ 42) * 13) % 10067;
    srand(k);
    return rand();
}

void assert(int x) {
    if (!x) {
        puts("Sorry but no flag");
        exit(1);
    }
}

void reset_seen() {
    for (int i = 0; i < 10; i ++) {
        seen[i] = 0;
    }
}

void check_flag() {
    assert(gen_value(0, 0, board[0][0]) == 1754831936);
    assert(gen_value(0, 4, board[0][4]) == 1322670498);
    assert(gen_value(0, 6, board[0][6]) == 2075469024);
    assert(gen_value(0, 7, board[0][7]) == 1924349448);
    assert(gen_value(1, 2, board[1][2]) == 1737338032);
    assert(gen_value(1, 4, board[1][4]) == 382094521);
    assert(gen_value(1, 5, board[1][5]) == 2003484635);
    assert(gen_value(1, 6, board[1][6]) == 1224890436);
    assert(gen_value(2, 4, board[2][4]) == 613863398);
    assert(gen_value(2, 5, board[2][5]) == 2131248558);
    assert(gen_value(2, 7, board[2][7]) == 1855404474);
    assert(gen_value(3, 0, board[3][0]) == 203716718);
    assert(gen_value(3, 2, board[3][2]) == 2132752585);
    assert(gen_value(4, 0, board[4][0]) == 54194304);
    assert(gen_value(4, 1, board[4][1]) == 548400147);
    assert(gen_value(4, 7, board[4][7]) == 2040844259);
    assert(gen_value(4, 8, board[4][8]) == 348846481);
    assert(gen_value(5, 6, board[5][6]) == 712829567);
    assert(gen_value(5, 8, board[5][8]) == 198917626);
    assert(gen_value(6, 1, board[6][1]) == 1999818593);
    assert(gen_value(6, 3, board[6][3]) == 47214827);
    assert(gen_value(6, 4, board[6][4]) == 117615071);
    assert(gen_value(7, 2, board[7][2]) == 1948118465);
    assert(gen_value(7, 3, board[7][3]) == 345110140);
    assert(gen_value(7, 4, board[7][4]) == 2113220118);
    assert(gen_value(7, 6, board[7][6]) == 443730372);
    assert(gen_value(8, 1, board[8][1]) == 2136198019);
    assert(gen_value(8, 2, board[8][2]) == 1427855150);
    assert(gen_value(8, 4, board[8][4]) == 323649682);
    assert(gen_value(8, 8, board[8][8]) == 1443247958);
    for (int r = 0; r < 9; r ++) {
        reset_seen();
        for (int c = 0; c < 9; c ++) {
            assert(board[r][c]);
            assert(!seen[board[r][c]]);
            seen[board[r][c]] = 1;
        }
    }
    for (int c = 0; c < 9; c ++) {
        reset_seen();
        for (int r = 0; r < 9; r ++) {
            assert(!seen[board[r][c]]);
            seen[board[r][c]] = 1;
        }
    }
    for (int a = 0; a < 3; a ++) {
        for (int b = 0; b < 3; b ++) {
            reset_seen();
            for (int r = 0; r < 3; r ++) {
                for (int c = 0; c < 3; c ++) {
                    assert(!seen[board[3 * a + r][3 * b + c]]);
                    seen[board[3 * a + r][3 * b + c]] = 1;
                }
            }
        }
    }
    puts("Wow you're good at sudoku!");
    print_flag();
}

int main() {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    start_color();
    use_default_colors();
    curs_set(0);
    init_pair(1, COLOR_WHITE, -1);
    init_pair(2, COLOR_CYAN, -1);
    init_pair(3, COLOR_CYAN, COLOR_RED);
    attron(COLOR_PAIR(1));
    for (int i = 0; i < 9 * 2 + 1; i++) {
        for (int j = 0; j < 9 * 4 + 1; j++) {
            mvaddch(i, j,
                    NATER(i % 2, j % 4, BLANK, HORIZ, VERT, CORNER) |
                        ((!(i % 6) || !(j % 12)) ? A_BOLD : 0));
        }
    }
    attroff(COLOR_PAIR(1));
    update_board();
    update_cursor(0, 0);
    while (1) {
        refresh();
        int inp = getch();
        if (inp == KEY_LEFT || inp == 'a') {
            update_cursor(cur_r, cur_c - 1);
        } else if (inp == KEY_RIGHT || inp == 'd') {
            update_cursor(cur_r, cur_c + 1);
        } else if (inp == KEY_DOWN || inp == 's') {
            update_cursor(cur_r + 1, cur_c);
        } else if (inp == KEY_UP || inp == 'w') {
            update_cursor(cur_r - 1, cur_c);
        } else if (inp == 'q' || inp == KEY_ENTER) {
            break;
        } else if (inp == '0' || inp == KEY_BACKSPACE || inp == ' ') {
            board[cur_r][cur_c] = 0;
            update_board();
            advance_cursor();
        } else if (is_number(inp)) {
            board[cur_r][cur_c] = char_to_num(inp);
            update_board();
            advance_cursor();
        }
    }
    delwin(stdscr);
    endwin();
    refresh();
    check_flag();
}