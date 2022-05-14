#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>

int main() {
    setbuf(stdout, NULL);
    gid_t gid = getegid();
    setresgid(gid, gid, gid);

    printf("> ");

    unsigned char * buf = mmap(0, 0x2000, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_PRIVATE | MAP_ANONYMOUS, 0, 0);

    int chars = read(0, buf, 0x2000);

    for(int i = 0; i < chars; ++i){
        if (buf[i] % 2 != i % 2){
            puts("bad shellcode!");
            return 1;
        }
    }

    void (*payload_ptr)() =  (void(*)())buf;
    payload_ptr();
    return 0;
}
