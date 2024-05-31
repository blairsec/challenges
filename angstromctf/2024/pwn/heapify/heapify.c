#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define N 32

int idx = 0;
char *chunks[N];

int readint() {
	char buf[0x10];
	read(0, buf, 0x10);
	return atoi(buf);
}

void alloc() {
	if(idx >= N) {
		puts("you've allocated too many chunks");
		return;
	}
	printf("chunk size: ");
	int size = readint();
	char *chunk = malloc(size);
	printf("chunk data: ");

	// ----------
	// VULN BELOW !!!!!!
	// ----------
	gets(chunk);
	// ----------
	// VULN ABOVE !!!!!!
	// ----------
	
	printf("chunk allocated at index: %d\n", idx);
	chunks[idx++] = chunk;
}

void delete() {
	printf("chunk index: ");
	int i = readint();
	if(i >= N || i < 0 || !chunks[i]) {
		puts("bad index");
		return;
	}
	free(chunks[i]);
	chunks[i] = 0;
}

void view() {
	printf("chunk index: ");
	int i = readint();
	if(i >= N || i < 0 || !chunks[i]) {
		puts("bad index");
		return;
	}
	puts(chunks[i]);
}

int menu() {
	puts("--- welcome 2 heap ---");
	puts("1) allocate");
	puts("2) delete");
	puts("3) view");
}

int main() {
	setbuf(stdout, 0);
	menu();
	for(;;) {
		printf("your choice: ");
		switch(readint()) { 
		case 1:
			alloc();
			break;
		case 2:
			delete();
			break;
		case 3:
			view();
			break;
		default:
			puts("exiting");
			return 0;
		}
	}
}
