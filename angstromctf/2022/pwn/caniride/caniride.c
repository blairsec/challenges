#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

char * drivers[4] = {
    "Aplet123",
    "lamchcl",
    "bosh",
    "cavocado"
};

char * desc[4] = {
    "speedy but will tell you about the 2000 calorie UCLA chili burger on the way there",
    "incredible",
    "bruhhhhh nahhhh bruhh fr bruh no way",
    "probably the best driver"
};

int main() {
    setbuf(stdout, NULL);
    gid_t gid = getegid();
    setresgid(gid, gid, gid);

    char name[50];
    
    char bio[1000];

    puts("Welcome to blairuber!");

    printf("Name: ");
    scanf("%49s", name);

    for(int i = 0; i < 4; ++i){
        printf("%d) %s - %s\n", i, drivers[i], desc[i]);
    }

    printf("Pick your driver: ");
    int idx = 0;
    scanf("%d", &idx);
    getchar();

    if (idx > 4){
        puts("Not enough drivers! Sorry.");
        exit(1);
    }

    sleep(3);

    printf("Hi, this is %s your driver. Get in!\n", drivers[idx]);

    sleep(3);

    printf("So... tell me a little about yourself: ");

    int n = read(0, bio, 999);
    bio[n] = '\x00';

    printf("Hmm, very interesting!\n");

    sleep(3);

    printf("Well we're here. Bye, ");
    printf(name);
    printf("!\n");

    exit(0);
}

