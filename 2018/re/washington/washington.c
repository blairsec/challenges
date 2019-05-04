#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
	/* Get # of rails */
	int rails;
	scanf("%d\n", &rails);

	/* Get the plaintext */
	char ptRaw[128];
	fgets(name, 129, stdin); ptRaw[strcspn(ptRaw, "\n")] = 0;
	int ptLen = strlen(ptRaw);

	/* Initialize randomness */
	srand(0x1337 ^ rails);

	/* XORs */
	char *pt = (char*)ptRaw;
	for (int i = 0; i < ptLen; i++)
	{
		pt[i] ^= 
	}

	/* Apply fences */
}
