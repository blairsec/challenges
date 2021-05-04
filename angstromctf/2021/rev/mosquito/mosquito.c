#include <stdio.h>
#include <stdlib.h>
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
void map_five(vec* buf1, vec* buf2, vec(*ptr)(vec, vec)) {
	for (int i = 0; i < 5; i++) {
		buf1[i] = ptr(buf1[i], buf2[i]);
	}
}
void unary_map_five(vec* buf1, vec(*ptr)(vec)) {
	for (int i = 0; i < 5; i++) {
		buf1[i] = ptr(buf1[i]);
	}
}
vec negate(vec v) {
	return -v;
}
vec xor(vec a, vec b) {
	return a ^ b;
}
vec mul1(vec b, vec a) {
	return a * b;
}
vec mul2(vec b, vec a) {
	return a * (b*b+1);
}
vec copy(vec a, vec b) {
	return b;
}
void no_blackboxing_here(vec *buf) {
	for (int i = 0; i < 40; i++) {
		int j = (i + 1) % 40;
		buf[i/8][i%8] -= buf[j/8][j%8];
	}
}
int main() {
	char buf[41];
        vec arr[5];
	printf("What to encrypt? ");
        fgets(buf, 41, stdin);
        for (int i = 0; i < 40; i++) {
                arr[i/8][i%8] = buf[i];
        }
	srand(1337);
	for (int i = 0; i < 7; i++) {
		cpuid(i);
	}
	vec arb1[5];
	vec arb2[5];
	vec arb2_2[5];
	gen_five(arb1);
	gen_five(arb2);
	map_five(arb2_2, arb2, &copy);
	no_blackboxing_here(arb1);
	no_blackboxing_here(arb2);
	no_blackboxing_here(arb2_2);
	no_blackboxing_here(arr);
	unary_map_five(arb1, &negate);
	no_blackboxing_here(arb1);
	map_five(arr, arb1, &xor);
	no_blackboxing_here(arr);
	map_five(arb2, arr, &mul1);
	no_blackboxing_here(arb2);
	map_five(arb2_2, arr, &mul2);
	no_blackboxing_here(arb2_2);
	FILE* outf = fopen("flag.enc", "w");
	for (int i = 0; i < 40; i++) {
		putc(arb2[i/8][i%8], outf);
		putc(arb2_2[i/8][i%8], outf);
	}
	fclose(outf);
}

