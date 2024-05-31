#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int coeffs[9];

typedef struct output {
	int a;
	int b;
	int c;
	int d;
	short e;
	short f;
	short g;
	short h;
	short i;
} output;

// actf{wow_you_successfully_passed_algebra_1_3bf3c5d6}
// 52 bytes long
// (x-44)(x-58)(x-5)(x+37)(x-17)(x+9)(x-6)(x+4)
// -1733624640 + 134045088 x + 122783468 x^2 - 12168520 x^3 - 1364231 x^4 + 121272 x^5 - 358 x^6 - 80 x^7 + x^8
//-1733624640 134045088 122783468 -12168520 -1364231 121272 -358 -80 1
// 4 4 4 4 2 2 2 2 2
int evaluate(int x) {
	long total = 0;
	long power = 1;
	for (int i = 0; i < 9; i++) {
		total += power * coeffs[i];
		power *= x;
	}
	return total;
}
char xorcode[53] = "\xa1\x9f\xde\xfe\xdb\x2a\x92\x70\xb3\xff\x3e\x72\xe7\x21\x33\x9c\x9a\x4b\xcb\xaa\xfc\x8b\xdc\x93\x78\x5f\xb0\x9d\xd9\xeb\xc5\x39\xa2\x66\x80\xe1\x34\x65\xca\x33\x19\xce\xa6\x1d\xda\xbf\xa9\x9d\x85\x9b\x37\x7d\x00";
int main() {
	printf("I'm practicing my math skills. Give me nine integers: ");
	scanf("%d %d %d %d %d %d %d %d %d", &coeffs[0],  &coeffs[1],  &coeffs[2],  &coeffs[3],  &coeffs[4],  &coeffs[5],  &coeffs[6],  &coeffs[7],  &coeffs[8]);
	printf("Hmm, let me think");
	fflush(stdout);
	usleep(300000);
	printf(".");
	fflush(stdout);
	usleep(300000);
	printf(".");
	fflush(stdout);
	usleep(300000);
	printf(".\n");
	usleep(500000);
	for (int i = -60; i < 60; i++) {
		int result = evaluate(i);
		if (i == 44 || i == 58 || i == 5 || i == -37 || i == 17 || i == -9 || i == 6 || i == -4) {
			if (result != 0) {
				printf("Those aren't the right numbers. Try again!\n");
				return 1;
			}
		} else {
			if (result == 0) {
				printf("Those aren't the right numbers. Try again!\n");
				return 1;
			}
		}
	}
	if (coeffs[8] != 1) {
		for (int i = 0; i < 9; i++) {
			coeffs[i] /= coeffs[8];
		}
	}
	printf("Correct! Here's the flag: ");
	output o;
	o.a = coeffs[0];
	o.b = coeffs[1];
	o.c = coeffs[2];
	o.d = coeffs[3];
	o.e = coeffs[4];
	o.f = coeffs[5];
	o.g = coeffs[6];
	o.h = coeffs[7];
	o.i = coeffs[8];
	unsigned char xorbuf[52];
	memcpy(xorbuf, (char*)&o, 26);
	memcpy(xorbuf+26, (char*)&o, 26);
	for (int i = 0; i < 52; i++) {
		xorbuf[i] ^= xorcode[i];
		printf("%c", xorbuf[i]);
	}
	printf("\n");
}
