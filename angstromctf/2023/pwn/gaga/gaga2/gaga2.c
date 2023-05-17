#include <stdio.h>
#include <stdlib.h>

void main(){
    setbuf(stdout, NULL);
    gid_t gid = getegid();
    setresgid(gid, gid, gid);
    
    char buf[48];

    puts("Awesome! Now there's no system(), so what will you do?!");
    printf("Your input: ");
    
    gets(buf);
}