#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int counter = 0;

int main(){
    setbuf(stdout, NULL);

    gid_t gid = getegid();
	setresgid(gid, gid, gid);

    char name[50];

    puts("I'm so lost.");
    printf("Who are you? ");

    if (counter > 0){
        exit(1);
    }
    counter++;

    gets(name);

    puts("I hope you find yourself too.");

    return 0;
}