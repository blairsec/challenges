#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/types.h>

int PIE;
int SECRET;
int TRIES;

void flag() {
	system("/bin/cat flag.txt");
}

int main() {
	setvbuf(stdin, NULL, _IONBF, 0);
	setvbuf(stdout, NULL, _IONBF, 0);

	gid_t gid = getegid();
	setresgid(gid, gid, gid);

	srand(&PIE); //PIE is secure random
	TRIES = 10;

	int guess;
	char str[50];
	do {
		SECRET = rand();
		printf("You have %d tries remaining. What's my secret number? ", TRIES--);
		gets(str);
		if ((guess = atoi(str)) == SECRET) break;
		printf("You're terrible at this. The number was %d.\n", SECRET);
	} while (TRIES != 0);
	
	if (guess == SECRET) puts("That's right! You should get a flag now, right?");
}
