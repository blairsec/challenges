#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define N 0x100

size_t readint() {
	char buf[0x10];
	int cnt = read(0, buf, 0xf);
	buf[cnt] = 0;
	return strtoul(buf, 0, 10);
}

int cmp(const void *a, const void *b) {
	size_t v1 = *(size_t*)a;
	size_t v2 = *(size_t*)b;
	if(v1 < v2) return -1;
	else if(v1 > v2) return 1;
	return 0;
}

int main() {
	setbuf(stdin, 0);
	setbuf(stdout, 0);
	size_t items[N];
	for(int i = 0; i < N; i++) {
		printf("%d: ", i);
		items[i] = readint();
	}
	qsort(items, N*2, sizeof(size_t), cmp);
	cmp(stdin+8, stdout+8);
}
