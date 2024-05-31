#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define FLAG_LEN 44
// 01234567890123456789012345678901234567890123
// actf{now_fork_over_your_100k_annual_tuition}

const unsigned int xorcode = 0x4a72b18c;
// check: first byte is ignored, second byte is flag byte, third byte is total word index (last bit ignored) and fourth byte is full word index
char flagbuffer[64];
int filler_word = 0;
int normal_word = 0;

char *filler_words[] = {
"I",
"of",
"it",
"then",
"to",
"who",
"so",
"and",
"if",
"me",
"the",
"which",
"is",
"from",
"that",
"through"};

FILE *database;

int check_word(char *word) {
	if (strcmp(word, filler_words[filler_word % 16]) == 0) {
		filler_word++;
		return 0;
	}
	rewind(database);
	word[strlen(word)+1] = '\x00';
	word[strlen(word)] = ':';
	int pass_idx = 0;
	while (!feof(database) && pass_idx < strlen(word)) {
		char c = fgetc(database);
		if (c == word[pass_idx]) {
			pass_idx++;
		} else {
			pass_idx = 0;
		}
	}
	if (feof(database)) return 1;
	char octalbuf[48];
	for (int i = 0; ; i++) {
		octalbuf[i] = fgetc(database);
		if (octalbuf[i] == ' ') {
			octalbuf[i] = 0;
			break;
		}
	}
	unsigned int code;
	sscanf(octalbuf, "%o", &code);
	code ^= xorcode;
	if ((code & 0xff000000) != (0xbc000000)) return 1;
	if ((code & 0xff) != normal_word) return 1;
	if (((code >> 8) & 0xff) != normal_word + filler_word) return 1;
	flagbuffer[normal_word++] = (code >> 16) & 0xff;
	return 0;
}

int main() {
	char wordbuffer[256];
	char *textbuffer = malloc(65536);
	printf("Welcome to the ångstromCTF college application portal! Please take time to answer all questions honestly and from the heart - there are no right answers.\n");
	printf("-------------------------------------------------------------------ESSAY QUESTION------------------------------------------------------------------------\n");
	printf("Describe a time you did something that contributed to your community (65535 characters max).\n");
	printf("(start typing here) ");
	fgets(textbuffer, 65535, stdin);
	database = fopen("admissions-database.txt", "r");
	char *textp = textbuffer;
	int result; 
	while (*textp) {
		int i = 0;
		while (isalpha(*textp)) wordbuffer[i++] = *(textp++);
		if (*textp) textp++;
		wordbuffer[i] = 0;
		result = check_word(wordbuffer);
		if (result) break;
	}
	if (result) {
		printf("Thank you for your interest in ångstromCTF. Unfortunately, we are unable to offer you a flag in our competition of 2024. Every year, we receive hundreds of qualified flag submissions, and we cannot accept them all.\n");
		printf("We wish you the best of luck in all of your future challenge-solving endeavors.\n");
	} else {
		printf("On behalf of the ångstromCTF admissions committee, it is my pleasure to offer you a flag in our competition of 2024. Your application stood out as highlighting the unique qualities that make you you, including hard work, perseverance, passion and a commitment to making the world a better place.\n");
		flagbuffer[normal_word++] = 0;
		printf("Again, congratulations, and the flag is: %s\n", flagbuffer);
	}
}
