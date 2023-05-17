#include <stdio.h>
#include <stdlib.h>

void main(){
    setbuf(stdout, NULL);
    setbuf(stdin, NULL);
    gid_t gid = getegid();
    setresgid(gid, gid, gid);

    system("ls /var/www/html/uploads");
}