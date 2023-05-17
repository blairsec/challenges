#include <stdio.h>
#include <stdlib.h>

char *gpatterns[128];

int match(char *pattern, char *text) {
	char *initext = text;
	while (1) {
		char next = *(pattern++);
		if (next == ')' || next == ']' || next == '\0') return text - initext;
		else if (next == '(' || next == '[') {
			char gname = next == '(' ? *(pattern++) : '_';
			gpatterns[gname] = pattern;
			while (!(*pattern == ')' && *(pattern+1) == gname || *pattern == ']')) pattern++;
			pattern += 2;
			if (next == '[') {
				int attempt;
				do {
					attempt = match(gpatterns[gname], text);
					if (attempt > -1) text += attempt;
				} while (attempt > -1);
				pattern--;
			}
		} else if (next == '\'') {
			char gname = *(pattern++);
			if (match(gpatterns[gname], text) != -1) break;
		} else if (next == '*') {
			pattern--;
			while (*pattern == '*') {
				pattern++;	
				char gname = *(pattern++);
				int attempt = match(gpatterns[gname], text);
				if (attempt > -1) {
					text += attempt;
					while (*pattern == '*') pattern += 2;
					goto unionsuccess;
				}
			}
			goto matchfail;
			unionsuccess:;
		} else if (next == ' ') {
			if (!*(text++)) break;
		} else if (next == '?') {
			char obt = *(text++);
			int good = 0;
			while (*pattern != '?') {
				if (*pattern == obt) good = 1;
				pattern++;
			}
			pattern++;
			if (good) goto unionsuccess;
			break;
		} else if (next == '.') {
			
		} else {
			char obt = *(text++);
			if (obt != next) break;
		}
	}
matchfail:
	return -1;
}
int main(int argc, char **argv) {
	return match(argv[1], argv[2]) == -1;
}
