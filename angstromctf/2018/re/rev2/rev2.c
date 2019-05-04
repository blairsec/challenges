
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main (int argc, char* argv[]){


	puts("Welcome to Rev2! You'll probably want to use a dissassembler or gdb.");
	printf("Level 1: What number am I thinking of: ");
	int guess;
	scanf("%d", &guess);

	if (guess != 4567){
		printf("Sorry, your guess of %d was incorrect. Try again!\n", guess);
		return 0;
	}

	printf("Level 2: Which two two-digit numbers will solve this level. Enter the two numbers separated by a single space (num1 should be the lesser of the two): ");
	int num1, num2;
	scanf("%d %d", &num1, &num2);
	if (num1 > 99 || num1 < 10 || num2 > 99 || num2 < 10 || num1 > num2){
		puts("Numbers do not meet specifications. Try again!");
		return 0;
	}
	int guessVal = num1 * num2;
	if (guessVal != 3431){
		printf("Sorry, your guess of %d and %d was incorrect. Try again!\n", num1, num2);
		return 0;
	}

	printf("Congrats, you passed Rev2! The flag is: actf{%d_%d_%d}\n", guess, num1, num2);

	return 0;

}




