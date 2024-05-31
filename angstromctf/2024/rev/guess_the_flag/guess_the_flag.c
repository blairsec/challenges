#include <stdio.h>
#include <string.h>

char secretcode[] = "`bugzbnllhuude^un^uid^md`ru^rhfohghb`ou^chu|";

int main() {
	char buffer[64];
	printf("Go ahead, guess the flag: \n");
	fgets(buffer, 63, stdin);
	for (int i = 0; i < strlen(buffer); i++) {
		buffer[i] ^= 1;
	}
	if (!strcmp(buffer, secretcode)) {
		printf("Correct! It was kinda obvious tbh.\n");
	} else {
		printf("Wrong. Not sure why you'd think it'd be that.\n");
	}
}

