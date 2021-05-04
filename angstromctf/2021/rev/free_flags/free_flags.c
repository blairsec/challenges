#include <stdio.h>
#include <string.h>

void print_flag() {
    char flag[256];
    FILE* flagfile = fopen("flag.txt", "r");
    if (flagfile == NULL) {
        puts("Could not find the flag file.");
    }
    fgets(flag, 256, flagfile);
    flag[strcspn(flag, "\n")] = '\0';
    puts(flag);
}

int main(void) {
    setvbuf(stdout, NULL, _IONBF, 0);
    puts("Congratulations! You are the 1000th CTFer!!! Fill out this short survey to get FREE FLAGS!!!");
    puts("What number am I thinking of???");
    int inpn;
    scanf("%d", &inpn);
    if (inpn != 31337) {
        puts("Wrong >:((((");
        return 1;
    }
    puts("What two numbers am I thinking of???");
    int a;
    int b;
    scanf("%d %d", &a, &b);
    if (a + b != 1142 || a * b != 302937) {
        puts("Wrong >:((((");
        return 1;
    }
    puts("What animal am I thinking of???");
    char inp[256];
    scanf(" %256s", inp);
    inp[strcspn(inp, "\n")] = '\0';
    if (strcmp(inp, "banana")) {
        puts("Wrong >:((((");
        return 1;
    }
    puts("Wow!!! Now I can sell your information to the Russian government!!!");
    puts("Oh yeah, here's the FREE FLAG:");
    print_flag();
    return 0;
}
