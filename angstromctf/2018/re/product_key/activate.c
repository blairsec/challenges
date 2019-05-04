#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

char pad[] = {119,60,30,107,57,19,34,15,36,2,115,89,103,100,33,115,23,30,109,91,4,102,101,81,91,67,87,39,14,106,15,109,47,1,72,68,59,72,94,128,78,31,39,17,51,70,51,74,37,94,51,50,40,96,110,0,6,31,40,103,67,125,87,50};

static void swapArr(int arr[], int a, int b)
{
	arr[a] ^= arr[b];
	arr[b] ^= arr[a];
	arr[a] ^= arr[b];
}

static int sumChars(char str[], int start, int end, int step)
{
	int sum = 0;
	for (int i = start; i < end; i += step) sum += str[i];

	return sum;
}

bool verify_key(char *name, char *email, char *key)
{
	/* Parse the key into six 4-char subkeys */
	int subkeys[6];

	int i = 0;
	char *seg = strtok(key, "-");
	while (seg != NULL)
	{
		subkeys[i++] = atoi(seg);
		seg = strtok(NULL, "-");
	}

	/* Malformed key */
	if (i != 6) return false;

	/* Pad name and email */
	int pad_pos = 0;
	char *strs[2] = {email, name};

	for (int x = 0; x < 2; x++)
	{
		char *str = strs[x];

		int slen = strlen(str);
		if (slen < 32)
		{
			int padlen = 32 - slen;
			for (int i = 0; i < padlen; i++) str[slen + i] = pad[pad_pos + i];
			pad_pos = (32 - slen);
			str[32] = 0;
		}
	}

	/* XOR the name and email */
	for (int i = 0; i < 32; i++)
	{
		email[i] = 0x5 ^ email[i];
		name[i] = 0xf ^ name[i];
	}

	/* Use name and email to modify the subkeys */
	for (int i = 0; i < 6; i++)
	{
		int s1 = sumChars(email, 0, 32, i+2);
		int s2 = sumChars(email, i+1, 32, i+2);
		subkeys[i] -= (s1*s2) % 10000;
		subkeys[i] += (sumChars(name, 0, 32, 2) - sumChars(name, 1, 32, 2)) << 2;
	}

	/* Swap some subkeys */
	swapArr(subkeys, 3, 4);
	swapArr(subkeys, 2, 5);
	swapArr(subkeys, 1, 5);
	swapArr(subkeys, 2, 3);
	swapArr(subkeys, 0, 5);
	swapArr(subkeys, 4, 5);

	/* Modify the subkeys */
	for (int i = 0; i < 6; i++)
	{
		subkeys[i] += sumChars(name, 0, 32, 1);
		subkeys[i] += sumChars(email, 0, 32, 1);
	}

	for (int i = 0; i < 6; i++)
	{
		subkeys[i] += sumChars(email, i*4, i*4 + 1, 1) % sumChars(name, i*4 + 2, i*4 + 3, 1);
	}

	/* Derive what the final answer should be */
	int mult[6] = {2, 4, 6, 8, 7, 5};
	int ans[6] = {
					sumChars(email, 0, 10, 1),
					sumChars(email, 10, 25, 1),
					sumChars(email, 25, 32, 1),
					sumChars(name, 0, 13, 1),
					sumChars(name, 13, 20, 1),
					sumChars(name, 20, 32, 1)
				 };
	for (int i = 0; i < 6; i++)
	{
		ans[i] = (mult[i] * ans[i]) % 10000;
	}

	/* Check the answer */
	bool ret = true;
	for (int i = 0; i < 6; i++)
	{
		if (subkeys[i] != ans[i]) ret = false;
	}

	return ret;
}

int main(int argc, char **argv)
{
	/* Have the user enter their name, email, and product key */
	char name[33], email[33], key[33];
	printf("Name: ");        fflush(stdout); fgets(name, 33, stdin); name[strcspn(name, "\n")] = 0;
	printf("Email: ");       fflush(stdout); fgets(email, 33, stdin); email[strcspn(email, "\n")] = 0;
	printf("Product key: "); fflush(stdout); fgets(key, 33, stdin); key[strcspn(key, "\n")] = 0;

	/* Check the product key */
	if (verify_key(name, email, key))
	{
		printf("Windows has been activated\n");
		return 0;
	}
	else
	{
		printf("Invalid product key\n");
		return -1;
	}
}
