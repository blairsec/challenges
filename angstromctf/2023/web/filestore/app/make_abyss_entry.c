#include <stdio.h>
#include <stdlib.h>
#include <pwd.h>
#include <grp.h>

#include <dirent.h>
#include <errno.h>

void main(){
    setbuf(stdout, NULL);
    setbuf(stdin, NULL);
    gid_t gid = getegid();
    setresgid(gid, gid, gid);

    char buf[65] = {0};
    char d[4] = {0};
    for (int i = 0; i < 64; i+=2){
        getrandom(d, 4);
        sprintf(&buf[i], "%02x", (int)(*d) & 0xff);
    }
    
    

    uid_t          uid;
    struct passwd *pwd;
    struct group  *grp;

    char file[100] = {0};
    sprintf(&file, "/abyss/%s", buf);

    if (mkdir(file, 0333) == -1){
        perror("mkdir fail");
    }

    pwd = getpwnam("ctf");
    uid = pwd->pw_uid;
    grp = getgrnam("ctf");
    gid = grp->gr_gid;
    
    if (chown(file, uid, gid) == -1) {
        perror("chown fail");
    }
    if (chmod(file, 0330) == -1) {
        perror("chmod fail");
    }

    puts(buf);
}