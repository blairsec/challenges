#include <stdio.h>

#define SIZE 8 // has to be even
#define WALLS 25

// between cell (fx, fy) and (tx, ty)
struct wall { int fx, fy, tx, ty; };

struct wall walls[WALLS] = {
    {-1, 1, 0, 1}, {-1, 3, 0, 3},
    {1, 4, 1, 5}, {1, 6, 1, 7},
    {1, 1, 2, 1}, {1, 4, 2, 4},
    {2, -1, 2, 0}, {2, 0, 2, 1}, {2, 3, 2, 4},
    {2, 5, 3, 5},
    {3, 5, 3, 6},
    {3, 0, 4, 0}, {3, 2, 4, 2}, {3, 7, 4, 7},
    {4, -1, 4, 0}, {4, 1, 4, 2},
    {5, 4, 5, 5}, {5, 7, 5, 8},
    {5, 2, 6, 2}, {5, 5, 6, 5},
    {6, 2, 6, 3},
    {6, 6, 7, 6},
    {7, 2, 7, 3}, {7, 7, 7, 8},
    {7, 5, 8, 5},
};

int can_move(int fx, int fy, int tx, int ty) {
    for (int i = 0; i < WALLS; i++) {
        if (
            walls[i].fx == fx &&
            walls[i].fy == fy &&
            walls[i].tx == tx &&
            walls[i].ty == ty
        ) return 0;

        if (
            walls[i].fx == tx &&
            walls[i].fy == ty &&
            walls[i].tx == fx &&
            walls[i].ty == fy
        ) return 0;
    }
    return 1;
}

// path in a cell
struct path { int dx1, dy1, dx2, dy2; };

// four choose two
struct path paths[6] = {
    {0, -1, 0, 1},
    {-1, 0, 1, 0},
    {-1, 0, 0, 1},
    {-1, 0, 0, -1},
    {0, -1, 1, 0},
    {1, 0, 0, 1},
};

int check_path(struct path path[SIZE][SIZE]) {
    int seen[SIZE][SIZE] = {0};

    // we begin at (0, 0) arbitrarily
    int x = 0;
    int y = 0;
    seen[x][y] = 1;

    while (1) {
        int nx = x + path[x][y].dx1;
        int ny = y + path[x][y].dy1;

        // if we walked off the board then we finished our leg
        if (nx < 0 || nx >= SIZE || ny < 0 || ny >= SIZE) break;

        if (seen[nx][ny]) {
            nx = x + path[x][y].dx2;
            ny = y + path[x][y].dy2;
        }

        // if we walked off the board then we finished our leg
        if (nx < 0 || nx >= SIZE || ny < 0 || ny >= SIZE) break;

        // path cannot form a loop
        if (seen[nx][ny]) return 0;

        // the path must link up properly
        // i.e. if i can reach (nx, ny) from (x, y)
        // then (nx, ny) must be able to reach (x, y)
        int nnx1 = nx + path[nx][ny].dx1;
        int nny1 = ny + path[nx][ny].dy1;
        int nnx2 = nx + path[nx][ny].dx2;
        int nny2 = ny + path[nx][ny].dy2;
        if ((nnx1 != x || nny1 != y) && (nnx2 != x || nny2 != y)) return 0;

        // the path cannot cross a wall
        if (!can_move(x, y, nx, ny)) return 0;

        seen[nx][ny] = 1;
        x = nx;
        y = ny;
    }

    // we do it again because (0, 0) is probably not the start
    x = 0;
    y = 0;
    while (1) {
        int nx = x + path[x][y].dx2;
        int ny = y + path[x][y].dy2;

        // if we walked off the board then we finished our leg
        if (nx < 0 || nx >= SIZE || ny < 0 || ny >= SIZE) break;

        if (seen[nx][ny]) {
            nx = x + path[x][y].dx1;
            ny = y + path[x][y].dy1;
        }

        // if we walked off the board then we finished our leg
        if (nx < 0 || nx >= SIZE || ny < 0 || ny >= SIZE) break;

        // path cannot form a loop
        if (seen[nx][ny]) return 0;

        // the path must link up properly
        // i.e. if i can reach (nx, ny) from (x, y)
        // then (nx, ny) must be able to reach (x, y)
        int nnx1 = nx + path[nx][ny].dx1;
        int nny1 = ny + path[nx][ny].dy1;
        int nnx2 = nx + path[nx][ny].dx2;
        int nny2 = ny + path[nx][ny].dy2;
        if ((nnx1 != x || nny1 != y) && (nnx2 != x || nny2 != y)) return 0;

        // the path cannot cross a wall
        if (!can_move(x, y, nx, ny)) return 0;

        seen[nx][ny] = 1;
        x = nx;
        y = ny;
    }

    // now we check that every cell was visited
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (!seen[i][j]) return 0;
        }
    }

    return 1;
}

// we encode pairs of paths in letters
char indices[36] = "abcdefghijklmnopqrstuvwxyz123456789_";
int key[SIZE * SIZE / 2] = {
    8, 18, 30, 23, 4, 30, 9, 35,
    0, 1, 31, 12, 5, 0, 26, 20,
    27, 31, 29, 9, 3, 29, 21, 17,
    14, 27, 16, 6, 22, 7, 16, 11,
};

int decode(struct path *p1, struct path *p2, char c, int j) {
    for (int i = 0; i < SIZE * SIZE; i++) {
        if (indices[i] == c) {
            int shifted = (i + j) % 36;

            struct path pa = paths[shifted / 6];
            p1->dx1 = pa.dx1;
            p1->dy1 = pa.dy1;
            p1->dx2 = pa.dx2;
            p1->dy2 = pa.dy2;

            struct path pb = paths[shifted % 6];
            p2->dx1 = pb.dx1;
            p2->dy1 = pb.dy1;
            p2->dx2 = pb.dx2;
            p2->dy2 = pb.dy2;

            return 1;
        }
    }
    return 0;
}

int decode_string(struct path path[SIZE][SIZE], char *s) {
    int s_index = 0;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j += 2) {
            if (
                !decode(
                    &path[i][j],
                    &path[i][j + 1],
                    s[s_index],
                    key[s_index]
                )
            ) return 0;
            s_index++;
        }
    }
    return 1;
}

int main() {
    char buf[64];
    char flag[SIZE * SIZE / 2 + 1] = {0};

    printf("flag: ");
    fflush(stdout);

    // idc about the macros anymore
    fgets(buf, 64, stdin);
    if (sscanf(buf, "actf{%32s%[}]", flag, buf) != 2) {
        printf("invalid input\n");
        return 1;
    }

    struct path path[SIZE][SIZE];
    if (!decode_string(path, flag)) {
        printf("incorrect\n");
        return 0;
    }

    if (check_path(path)) {
        printf("correct\n");
    } else {
        printf("incorrect\n");
    }
}
