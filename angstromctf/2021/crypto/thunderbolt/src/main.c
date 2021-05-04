#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define x(a, b)              \
    ((a) ^= (b), (b) ^= (a), \
     (a) ^= (b))

int s = 0;
int f[256];

void fgen(unsigned char* k, unsigned char* iv, size_t kl, size_t ivl) {
    for (int i = 0; i < 256; i++) {
        f[i] = i;
    }
    for (int m = 0; m < 768; m++) {
        s = f[(s + f[m % 256] + k[m % kl]) % 256];
        x(f[m % 256], f[s]);
    }

    for (int m = 0; m < 768; m++) {
        s = f[(s + f[m % 256] + iv[m % ivl]) % 256];
        x(f[m % 256], f[s]);
    }
}

void enc(unsigned char* msg, unsigned char* e, size_t length) {
    int n = 0;
    for (int i = 0; i < length; i++) {
        s = f[(s + f[n]) % 256];
        e[i] = msg[i] ^ f[(f[f[s]] + 1) % 256];
        x(f[n], f[s]);
        n += 1;
        n %= 256;
    }
}

int main() {
    setvbuf(stdout, NULL, _IONBF, 0);

    char* inp = NULL;
    printf("Enter a string to encrypt: ");
    size_t _n = 0;
    getline(&inp, &_n, stdin);

    int len = strcspn(inp, "\n");

    char flag[256];
    FILE* flagfile = fopen("flag", "rb");
    if (flagfile == NULL) {
        puts("Could not open flag file.");
        return 1;
    }
    fgets(flag, 256, flagfile);
    int flaglen = strcspn(flag, "\n");
    flag[flaglen] = '\0';

    char* buf = realloc(inp, len + flaglen + 1);
    strcpy(buf + len, flag);

    len += flaglen;

    FILE* randfile;
    randfile = fopen("/dev/urandom", "rb");

    char iv[16];
    fread(iv, 16, 1, randfile);

    char* key = malloc(len);
    fread(key, len, 1, randfile);

    fgen((unsigned char*)key, (unsigned char*)iv, len, 16);

    unsigned char* out = malloc(len);
    enc((unsigned char*)buf, out, len);
    for (int i = 0; i < len; i++) {
        printf("%02x", out[i]);
    }
    printf("\n");

    free(key);
    free(buf);
    free(out);
}
