#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main (int argc, char* argv[]){

	puts("Welcome to your first Reverse Engineering challenge!");
	printf("What is the password to this file? Enter password here: ");

	char *name = "s3cret_pa55word";
	char guess[64];
	
	fgets(guess, 64, stdin);
	guess[strlen(guess) - 1] = '\0';

	if (strcmp(name, guess)){
		printf("Sorry, the password isn't %s. Try again!\n", guess);
	} else {
		puts("Correct! You read my mind, have a flag: ");
		gid_t gid = getegid();
		setresgid(gid,gid,gid);
		system("/bin/cat flag");
	}
	
	return 0;

}
