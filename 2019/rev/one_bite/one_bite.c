#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, char** argv) {

	
	puts("Give me a flag to eat: ");

	char buf[34];
	fgets(buf, 34, stdin);
	for (int i = 0; i < strlen(buf); i++) {
		buf[i] = buf[i] ^ 0x3c;
	}


	char *enc = "]_HZGUcHTURWcUQc[SUR[cHSc^YcOU_WA";
	
	if (!strcmp(buf, enc)) {
		puts("Yum, that was a tasty flag.");
	}
	else {
		puts("That didn't taste so good :(");
	}

	return 0;
}