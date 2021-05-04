#include <stdlib.h>
#include <stdio.h>
void cpuid(unsigned int mode) {
        unsigned int eax, ebx, ecx, edx;
        eax = mode;
        asm volatile("cpuid": "=a" (eax), "=b" (ebx), "=c" (ecx), "=d" (edx): "0" (eax), "2" (ecx));
        srand(rand() + eax);
        srand(rand() + ((mode == 1) ? 0 : ebx));
        srand(rand() + ecx);
        srand(rand() + edx);
}
typedef unsigned char vec __attribute__ ((vector_size (8)));
vec gen_one() {
        vec val = {(char)rand(), (char)rand(), (char)rand(), (char)rand(),(char)rand(), (char)rand(),(char)rand(), (char)rand()};
        return val;
}
void gen_five(vec* buf) {
        for (int i = 0; i < 5; i++) {
                buf[i] = gen_one();
        }
}
void no_blackboxing_here(vec *buf) {
        for (int i = 0; i < 40; i++) {
                int j = (i + 1) % 40;
                buf[i/8][i%8] -= buf[j/8][j%8];
        }
}
void f(vec *buf) {
	buf[4][7] += buf[0][0];
	for (int i = 39; i >= 0; i--) {
		int j = i + 1;
                buf[i/8][i%8] += buf[j/8][j%8];
	}
}
vec final1[5];
vec final2[5];
int main() {
	vec arb1o[5];
	vec arb2o[5];
	srand(1337);
	for (int i = 0; i < 7; i++) {
		cpuid(i);
	}
	gen_five(arb1o);
	gen_five(arb2o);
	no_blackboxing_here(arb1o);
	no_blackboxing_here(arb2o);
	FILE *enc = fopen("flag.enc", "r");
	for (int i = 0; i < 40; i++) {
		final1[i/8][i%8] = getc(enc);
		final2[i/8][i%8] = getc(enc);
	}
	f(final1);
	f(final2);
	vec int1[5];
	for (int i = 0; i < 40; i++) {
		unsigned char b = arb2o[i/8][i%8];
		unsigned char o1 = final1[i/8][i%8];
		unsigned char o2 = final2[i/8][i%8];
		unsigned char val = 0;
		for (unsigned int ai = 0; ai < 256; ai++) {
			unsigned char a = (unsigned char) ai;
			unsigned char h1 = a*b;
			unsigned char h2 = a*(b*b+1);
			if (h1 == o1 && h2 == o2) {
				val = a;
				break;
			}
		}
		if (val) {
			int1[i/8][i%8] = val;
		} else {
			printf("error debug report this %x %x %x\n", b, o1, o2);
		}
	}
	for (int i = 0; i < 40; i++) {
		arb1o[i/8][i%8] *= -1;
	}
	no_blackboxing_here(arb1o);
	f(int1);
	for (int i = 0; i < 40; i++) {
		int1[i/8][i%8] ^= arb1o[i/8][i%8];
	}
	f(int1);
	for (int i = 0; i < 40; i++) {
		printf("%c", int1[i/8][i%8]);
	}
	printf("\n");
}

