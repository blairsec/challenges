#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, char** argv) {

	puts("I like the string that I'm thinking of: ");
	
	char buf[20];
	fgets(buf, 20, stdin);
	buf[strlen(buf) - 1] = '\0';

	if (strcmp(buf, "okrrrrrrr")) {
		puts("Cardi don't like that.");
		exit(0);
	}

	puts("I said I like it like that!");

	puts("I like two integers that I'm thinking of (space separated): ");
	int guess1;
	int guess2;

	char num[12];
	fgets(num, 12, stdin);
	sscanf(num, "%d %d", &guess1, &guess2);

	if ((guess1 + guess2) != 136 || guess1 * guess2 != 3783 || guess1 >= guess2) {
		puts("Cardi don't like that.");
		exit(0);
	}

	puts("I said I like it like that!");
	printf("Flag: actf{%s_%d_%d}\n", buf, guess1, guess2);


}