#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void shuffleChars(char *string, int a, int b);
void shuffleCharsCmd(char *string, int a, int b, int *commands);

#define POPCNT(N) ((1 << (N)) - 1)

#define CMD_END POPCNT(0)
#define CMD_SHUF POPCNT(12)
#define CMD_COPY POPCNT(13)
#define CMD_SHUFP POPCNT(14)
#define CMD_NOP POPCNT(15)
#define CMD_DOUBLE POPCNT(16)
#define CMD_SHUFM POPCNT(17)
#define CMD_ALLOC POPCNT(18)
#define CMD_MULT(N) POPCNT(N+11)

int table[] = {0xae915fca, 0x83bf5261, 0xb5ec498c, 0xf51485ba, 0xa3ba4f06, 0xef32d0a4, 0x5026d50b, 0xb6f82f01, 0x09370e3f, 0x4e8cd6e4, 0x0517c57e, 0xb70823aa, 0xba6c4e8a, 0x8dc7a316, 0x712d5c59, 0xa87433a7, 0xa48ab39d, 0x44fea022, 0x6616fc91, 0x52bdbc0c, 0x16a371f8, 0x71d985a9, 0xc8e1088f, 0x648e2dcd, 0xde2d4c43, 0xc30f157a, 0xafe2e4e0, 0x3887e365, 0x97aa14d5, 0x5a289878, 0x4b47e1e8, 0x64c39373, 0x76a8dbee, 0x63d55b11, 0x27f6a924, 0x8f3f3111, 0x583b79c2, 0xb9f0b036, 0xbc249505, 0x326cbbd2, 0xa84717cb, 0x6f0b3607, 0x36ac2eac, 0xc86a3da3, 0x45f26910, 0x8bea8b61, 0x626a8358, 0x3ae7c246, 0xd2d6026f, 0x66319844, 0x5d029e79, 0xa59aba2a, 0xd104646e, 0x9b45e8c3, 0x9839eb23, 0xf60555c3, 0xff3c500a, 0x00000000, 0x7c95505d, 0x4f586c1b, 0x777c80c6};
int main() {
	char str[] = "actf{l_plus_ratio_plus_no_flags}";
	char xor_code[32] = {0x4f, 0x93, 0xd8, 0xcc, 0x7d, 0x0c, 0x4c, 0x44, 0xaf, 0x6c, 0x4f, 0xf9, 0x0b, 0x47, 0xfb, 0x74, 0x42, 0x59, 0xc2, 0x04, 0x1e, 0xa5, 0x73, 0xad, 0x4e, 0xe8, 0x50, 0x3e, 0x34, 0x62, 0x73, 0xcf};
	shuffleCharsCmd(str, 1, 1, table);
	for (int i = 0; i < 32; i++) {
		str[i] ^= xor_code[i];
	}
	printf("%32s\n", str);
}

void shuffleChars(char *string, int a, int b) {
	srand(a);
	long l = 0;
	for (int i = 0; i < 32; i++) {
		long oldl = l + rand();
		l *= b;
		char temp = string[oldl % 32];
		string[oldl % 32] = string[l % 32];
		string[l % 32] = temp;
	}
}
void shuffleCharsCmd(char *string, int a, int b, int *commands) {
	char *buf;
	int *pt0, *pt1, *pt2, *pt3;
	long bufferSize = 32;
	int ct = 0;
	int test;
	while (*commands) {
		int val = 0;
		int cmd = *(commands++);
		asm ("popcnt %1, %0" : "=r" (val) : "r" (cmd));
		val -= 11;
		if (val > 8) {
			for (*pt1 = 0; *pt1 < bufferSize/2; (*pt1)++) {
				buf[*pt1] *= val;
			}
			continue;
		}
		switch (val) {
			case 0:
				break;
			case 1:
				shuffleChars(string, a, b);
				break;
			case 2:
				memcpy(buf + 32*ct, string, 32);
				ct++;
				pt3 = malloc(bufferSize);
				break;
			case 3:
				shuffleChars(string, a+1, b+1);
				pt2 = malloc(test);
				*pt2 = test;
				break;
			case 4:
				break;
			case 5:
				buf = realloc(buf, bufferSize);
				bufferSize *= 2; 
				pt0 = realloc(pt0, bufferSize);
				break;
			case 6:
				shuffleChars(string, a-1, b-1);
				break;
			case 7:
				bufferSize = 32;
				buf = malloc(bufferSize*8);
				bufferSize *= 2;
				pt1 = malloc(sizeof(int) << 32); // 32 bit int
				break;
			default:
				val = 0;
				commands = &val;
		}
	}
	for (*pt0 = 0; *pt0 < 32; (*pt0)++) {
		string[*pt0] = 0;
	}
	*pt2 = 0;
	for (*pt3 = 0; *pt3 < bufferSize/2; (*pt3)++) {
		string[*pt2 % 32] ^= buf[*pt3];
		*pt2 = *pt3 + (*pt0 >> 5);
	}
}
