#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {
	int a = 0xabcdef01;
	srand(a ^ a);
	// flag in vec3
	// encryption process
	// generate two encryption vectors of same length as flag
	// perform bytewise xor with first nybble of first vector and of second vector
	// second nybble of first vector is added
	// second nybble of second vector is used to swap byte with that amount ahead, wrapping
	// nybbles are read out in reverse
	int flen = strlen(argv[1]);
	unsigned char *vec1, *vec2, *vec3;
	vec1 = malloc(3*flen);
	vec2 = vec1 + flen;
	vec3 = vec2 + flen;
	for (int i = 0; i < flen; i++) {
		unsigned char b1, b2;
		unsigned short r = rand() & 0xffff;
		for (int j = 0; j < 8; j++) {
			b1 <<= 1;
			b1 |= r & 1;
			r >>= 1;
			b2 <<= 1;
			b2 |= r & 1;
			r >>= 1;
		}
		vec1[i] = b1;
		vec3[i] = argv[1][i];
		vec2[i] = b2;
	}
	
	for (int i = 0; i < flen; i++) {
		vec3[i] ^= (vec1[i] & 0xf) | (vec2[i] & 0xf0);
		vec3[i] += vec1[i] >> 4;
	}
	for (int i = 0; i < flen; i++) {
		int j = i + (vec2[i] & 0xf);
		if (j >= flen) j -= flen;
		unsigned char t = vec3[j];
		vec3[j] = vec3[i];
		vec3[i] = t;
	}
	for (int i = flen-1; i >= 0; i--) {
		printf("%02x", vec3[i]);
	}
	printf("\n");
}
