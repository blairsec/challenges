#include <stdio.h>
#include <stdlib.h>
#include "fxc.h"

unsigned char* lookup;
int (*closure(int a, int b, int c, unsigned short *tbl))(int);
int main() {
	unsigned long x;
	FILE *flag_file = fopen("flag", "r");
	char flag[64];
	fgets(flag, 64, flag_file);
	FILE *table = fopen("not_flag", "r");
	lookup = malloc(131072);
	fread(lookup, 131072, 1, table);
	int flag_len = strlen(flag);
	int int_flag[64];
	FILE *urandom = fopen("/dev/urandom", "r");
	for (int i = 0; i < flag_len; i++) {
		int salt = 0;
		for (int j = 0; j < 3; j++) {
			salt += fgetc(urandom);
			salt <<= 8;
		}
		int_flag[i] = salt + flag[i];
	}
	int (*c1)(int) = closure(0xdead, 0xbeef, 0xfeed, (unsigned short*)lookup);
	int (*c2)(int) = closure(0x1337, 0xcafe, 0x1234, (unsigned short*)lookup);
	int (*both1)(int) = compose(c1, c2);
	int (*both2)(int) = compose(c2, c1);
	int *flag2 = map(both1, int_flag, flag_len);
	int *flag3 = map(both2, flag2, flag_len);
	for (int i = 0; i < flag_len; i++) {
		printf("%x ", flag3[i]);
	}
	printf("is the encrypted flag. Good luck!\n");
	free(lookup);
}
int (*closure(int a, int b, int c, unsigned short *tbl))(int) {
	unsigned long x;
	LAMBDA(lambda, a ^ (tbl[(x & 0xffff) ^ b] + (tbl[(x >> 16) ^ b] << 16)));
	return lambda;
}
