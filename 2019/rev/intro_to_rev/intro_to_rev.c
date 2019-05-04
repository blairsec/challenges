#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, char** argv) {
	gid_t gid = getegid();
	setresgid(gid, gid, gid);

	puts("Welcome to your first reversing challenge!\n");
		puts("If you are seeing this, then you already ran the file! Let's try some input next.");

	char buf[20];
	puts("Enter the word 'angstrom' to continue: ");
	fgets(buf, 20, stdin);
	buf[strlen(buf) - 1] = '\0';

	if (strcmp(buf, "angstrom")) {
		puts("That wasn't right, try again.");
		exit(0);
	}

	puts("Good job! Some programs might also want you to enter information with a command line argument.\n");
	puts("When you run a file, command line arguments are given by running './introToRev argument1 argument2' where you replace each argument with a desired string.\n");
	
	if (argc != 3) {
		puts("To get the flag for this problem, run this file again with the arguments 'binary' and 'reversing' (don't put the quotes).\n");
		exit(0);
	}
	else {
		if (strcmp("binary", argv[1]) || strcmp("reversing", argv[2])) {
			puts("The arguments should be 'binary' and 'reversing'.");
		}
		else {
			puts("Good job, now go solve some real problems!");
			system("/bin/cat flag.txt");
		}
	}

	return 0;
}