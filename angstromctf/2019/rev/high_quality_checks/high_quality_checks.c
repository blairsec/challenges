#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char notFlag[] = "flag{n0t_th3_f14g}";

int n (int m) {
	return m >> 1;
}

int o (char c) {
	int r = c << 0x8;
	if (c < 0x61) {
		r += c - '0';
	} else {
		r += c - 'a' + 10;
	}
	return (r << 0x10) + r;
}

int v (char c) {
	return (c ^ 0x37) == n(0xac);
}

int w (char * s) {
	char c = *(s + 2);
	char b = *(s + 1);
	char a = *s;
	int d = (c << 0x10) + (b << 0x8) + a;
	return ~d == 0xff998b9c;
}

int e (int i) {
	return (i % 4) / 2;
}

int b (char * s, int o) {
	return *(s + o) == n(0xf6) + (e(o) << 1);
}

int k (char c) {
	return o(c) != 0x660f660f;
}

int z (char * s, char o) {
	char o1 = 0;
	char o2 = 0;
	for (int i = 0; i < 8; i ++) {
		char b = (o & (1 << i)) >> i;
		if (i % 2) {
			o1 += b << (i / 2);
		} else {
			o2 += b << (i / 2);
		}
	}
	return (*(s + o1) == 0x75) && (*(s + o1 + 1) == n(0xdc) && (*(s + o2) == n(0xea))) && (*(s + o2 + 1) == 0x6e);
}

int s (char * k) {
	int r = 0;
	for (int i = 0; i < 19; i ++) {
		if (o(*(k + i)) == 0x5f2f5f2f) {
			r += i + 1;
		}
	}
	return r == 9;
}

int d (int * v) {
	return *v == 0x30313763;
}
int u (char a, char b) {
	return (a == n(0xdc)) && (o(b) == 0x35053505);
}

int check (char * str) {
	return d((int *)(str + 12)) && v(*str) && u(str[16], str[17]) && !(k(str[5]) || k(str[9])) && w(str + 1) && b(str, 18) && b(str, 4) && z(str, 'l') && s(str);
}

int main () {
	char input [20];
	printf("Enter your input:\n");
	scanf("%19s", input);
	if (strlen(input) < 19) {
		printf("Flag is too short.\n");
		return 0;
	}
	if (check(input)) {
		printf("You found the flag!\n");
	} else {
		printf("That's not the flag.\n");
	}
	return 0;
}