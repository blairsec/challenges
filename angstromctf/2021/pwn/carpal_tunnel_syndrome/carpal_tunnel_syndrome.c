#include <stdio.h>

#include <stdlib.h>

#include <string.h>

#include <unistd.h>

#include <stdbool.h>


char * texts[] = {
    "Drama in the Discord server",
    "Solution dependent on raw manpower",
    ">30% downtime",
    "Prizes are delayed >1month",
    "No source code",
    "Solution requires $$$ cloud cluster",
    "<20 CTFtime rating",
    "Every admin asleep",
    ">1 OSINT challenge",
    "Solution uses author's tool",
    "Forgot to upload files",
    "Fake flags",
    "FREE",
    "Guessing",
    "Stolen/recycled challenges",
    "Challenge retracted after solved",
    "Stego",
    "Registration closed after CTF starts",
    "Broken reversing challenge",
    "\"Scoreboard is frozen\"",
    "Releasing hints after first solve",
    "Blind pwn challenge",
    "No flag format",
    "Flags/chals get leaked",
    "CTF infrastructure gets hacked",
};

char marker[25];

typedef struct Slot {
    char marked[8];
    struct Slot * right;
    struct Slot * bottom;
    char * text;
}
Slot;

Slot * root = NULL;

void setup() {
    Slot * heads[5];

    for (int i = 0; i < 5; ++i) {
        heads[i] = malloc(sizeof(Slot));
        Slot * curr = heads[i];

        curr -> text = texts[i * 5];

        for (int j = 1; j < 5; ++j) {
            Slot * new = malloc(sizeof(Slot));
            new -> text = texts[i * 5 + j];

            curr -> right = new;
            curr = new;
        }
    }

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 5; ++j) {
            Slot * curr = heads[i];
            Slot * bottom_curr = heads[i + 1];

            for (int k = 0; k < j; ++k) {
                curr = curr -> right;
                bottom_curr = bottom_curr -> right;
            }

            curr -> bottom = bottom_curr;
        }
    }

    root = heads[0];
}

void mark() {
    int x = 0, y = 0;
    printf("Enter coordinates x and y separated by a space: ");

    scanf("%d %d", & x, & y);

    Slot * node = root;

    for (int i = 0; i < x; ++i) {
        node = node -> right;
        if (node == NULL) {
            puts("Out of bound.");
            exit(1);
        }
    }

    for (int i = 0; i < y; ++i) {
        node = node -> bottom;

        if (node == NULL) {
            puts("Out of bound.");
            exit(1);
        }
    }

    memcpy(node -> marked, & marker, 8);

    puts("Marked!");

}

void view() {
    int x = 0, y = 0;
    printf("Enter coordinates x and y separated by a space: ");

    scanf("%d %d", & x, & y);

    if (x < 0 || x > 4 || y < 0 || y > 4) {
        puts("Out of bounds.");
        exit(1);
    }

    Slot * node = root;

    for (int i = 0; i < x; ++i) node = node -> right;
    for (int i = 0; i < y; ++i) node = node -> bottom;

    printf("Slot at (%d, %d): %s\n", x, y, node -> text);
}

void remove_bingo(Slot * head, int d) {
    if (d == 0) {
        // horizontally remove

        for (int i = 0; i < 5; ++i) {
            Slot * temp = head;
            head = temp -> right;
            free(temp);
        }

    } else if (d == 1) {
        // vertically remove

        for (int i = 0; i < 5; ++i) {
            Slot * temp = head;
            head = temp -> bottom;
            free(temp);
        }

    }
}

void winner() {
    puts("Congratulations on getting a bingo! Leave your name on the hall of fame here.");
    printf("Length of your name: ");

    int len = 0;
    scanf("%d", & len);

    char * name = malloc(len + 1);

    printf("Name: ");

    read(0, name, len);

    name[strcspn(name, "\n")] = 0;

    printf("Once again, congratulations, %s!\n", name);
}

void reset() {
    char choice[9];

    int idx = 0;
    printf("Index to reset: ");
    scanf("%d", & idx);

    if (idx < 0 || idx > 4) {
        puts("Invalid index.");
        exit(1);
    }

    printf("(r)ow / (c)olumn: ");
    scanf("%8s", & choice);

    if (choice[0] == 'r') {
        // row

        Slot * head = malloc(sizeof(Slot));
        head -> text = texts[idx * 5];

        Slot * c = head;

        for (int j = 1; j < 5; ++j) {
            Slot * new = malloc(sizeof(Slot));
            new -> text = texts[idx * 5 + j];

            c -> right = new;
            c = c -> right;
        }

        if (idx != 0) {
            Slot * curr = head;
            Slot * top = root;

            for (int j = 0; j < idx - 1; ++j) top = top -> bottom;

            for (int j = 0; j < 5 && top != NULL; ++j) {
                top -> bottom = curr;

                curr = curr -> right;
                top = top -> right;
            }
        }

        if (idx != 4) {
            Slot * curr = head;
            Slot * bottom = root;

            for (int j = 0; j < idx + 1; ++j) bottom = bottom -> bottom;

            for (int j = 0; j < 5 && bottom != NULL; ++j) {
                curr -> bottom = bottom;

                curr = curr -> right;
                bottom = bottom -> right;
            }
        }

    } else if (choice[0] == 'c') {
        // column
        Slot * head = malloc(sizeof(Slot));
        head -> text = texts[idx];

        Slot * c = head;

        for (int j = 1; j < 5; ++j) {
            Slot * new = malloc(sizeof(Slot));
            new -> text = texts[idx + j * 5];

            c -> bottom = new;
            c = c -> bottom;
        }

        if (idx != 0) {
            Slot * curr = head;
            Slot * left = root;

            for (int j = 0; j < idx - 1; ++j) left = left -> right;

            for (int j = 0; j < 5 && left != NULL; ++j) {
                left -> right = curr;

                curr = curr -> bottom;
                left = left -> bottom;
            }
        }

        if (idx != 4) {
            Slot * curr = head;
            Slot * right = root;

            for (int j = 0; j < idx + 1; ++j) right = right -> right;

            for (int j = 0; j < 5 && right != NULL; ++j) {
                curr -> right = right;

                curr = curr -> bottom;
                right = right -> bottom;
            }
        }

    } else {
        puts("Invalid choice.");
        exit(1);
    }
}

void menu() {
    puts("--- Menu ---");
    puts("1 -> Mark slot");
    puts("2 -> View board");
    puts("3 -> Reset row / column");
    puts("4 -> Check specific index for bingo");
    puts("5 -> Check for bingos");
    puts("6 -> Change your marker");
}

void check() {
    // check horizontally
    for (int i = 0; i < 5; ++i) {
        Slot * curr = root;

        for (int k = 0; k < i; ++k) curr = curr -> bottom;

        Slot * head = curr;

        int cnt = 0;

        for (int j = 0; j < 5; ++j) {
            if (strlen(curr -> marked) > 0 && strncmp(curr -> marked, marker, strlen(marker)) == 0) cnt++;

            curr = curr -> right;
        }

        if (cnt == 5) {
            puts("Horizontal bingo!");

            printf("Would you like to delete the row (y/n)? ");

            char choice[9];

            scanf("%8s", & choice);

            if (choice[0] == 'y') remove_bingo(head, 0);

            winner();

            return;
        }
    }

    // check vertically
    for (int i = 0; i < 5; ++i) {
        Slot * curr = root;

        for (int k = 0; k < i; ++k) curr = curr -> right;

        Slot * head = curr;

        int cnt = 0;

        for (int j = 0; j < 5; ++j) {
            if (strlen(curr -> marked) > 0 && strncmp(curr -> marked, marker, strlen(marker)) == 0) cnt++;

            curr = curr -> bottom;
        }

        if (cnt == 5) {
            puts("Vertical bingo!");

            printf("Would you like to delete the column (y/n)? ");

            char choice[9];

            scanf("%8s", & choice);

            if (choice[0] == 'y') remove_bingo(head, 1);

            winner();

            return;
        }
    }
}

void check_specific() {
    printf("Index to check: ");

    int idx = 0;
    scanf("%d", & idx);

    if (idx < 0 || idx > 4) {
        printf("Out of bounds.");
        exit(1);
    }

    char choice[9];

    printf("(r)ow / (c)olumn: ");
    scanf("%8s", & choice);

    if (choice[0] == 'r') {
        Slot * curr = root;

        for (int i = 0; i < idx; ++i) curr = curr -> bottom;

        int cnt = 0;

        for (int j = 0; j < 5; ++j) {
            if (strlen(curr -> marked) > 0 && strncmp(curr -> marked, marker, strlen(marker)) == 0) cnt++;

            curr = curr -> right;
        }

        if (cnt == 5) {
            puts("Horizontal bingo!");
            return;
        } else {
            puts("Nope!");
        }

    } else if (choice[0] == 'c') {

        Slot * curr = root;

        for (int i = 0; i < idx; ++i) curr = curr -> right;

        int cnt = 0;

        for (int j = 0; j < 5; ++j) {
            if (strlen(curr -> marked) > 0 && strncmp(curr -> marked, marker, strlen(marker)) == 0) cnt++;

            curr = curr -> bottom;
        }

        if (cnt == 5) {
            puts("Horizontal bingo!");
            return;
        } else {
            puts("Nope!");
        }

    } else {
        printf("Invalid choice.");
        exit(1);
    }

}

void change_marker() {
    printf("New marker: ");
    read(0, & marker, 24);
}

void main() {
    setbuf(stdout, NULL);
    setbuf(stderr, NULL);

    puts("Welcome to CTF Bingo!");
    printf("As with normal bingo, you may choose your marker now: ");

    read(0, & marker, 24);

    setup();

    while (1) {
        menu();

        printf("Choice: ");

        int choice = 0;
        scanf("%d", & choice);

        getchar();

        switch (choice) {
        case 1:
            mark();
            break;
        case 2:
            view();
            break;
        case 3:
            reset();
            break;
        case 4:
            check_specific();
            break;
        case 5:
            check();
            break;
        case 6:
            change_marker();
            break;
        }
    }

}
