#include <stdio.h>
#include <stdlib.h>

char arr[20];

void main(){
    setbuf(stdout, NULL);
    char name[16];

    printf("Name: ");
    fgets(name, 15, stdin);

    short pos = 0;
    arr[pos] = 1;

    int stop = 0;
    
    while (!stop) {
        int choice = 0;
        scanf("%d", &choice);
        getchar();


        switch(choice) {
            case 0:
                if (pos == 0) {
                    puts("hey!");
                    exit(1);
                }
                pos--;
            case 1:
                if (pos == 2147483647) {
                    puts("hey!");
                    exit(1);
                }
                pos++;
                break;
            case 2:
                arr[pos] = getchar();
                break;
            case 3:
                stop = 1;
                break;
        }

        for (int i = 0; i < 20; ++i) {
            printf("%s%s", arr[i] ? "x" : " ", (i == 0 || i == 20) ? "" : "|");
        }
        printf("\n");
    }

    puts("bye");
    puts(name);
}