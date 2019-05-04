#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void encode(char *inp, char *result){

	int inputLength = strlen(inp);

	for (int i = 0; i < inputLength; i++){
		
		result[i] = (char)((inp[i] ^ 9) - 3);
		
	}
}


int main(int argc, char* argv[]){
	
	if (argc != 2){
		puts("Usage: ./rev1 <inputString>");
		return 0;
	}

	char result[strlen(argv[1])];
	encode(argv[1], result);
	
	result[strlen(result)-1]='\0';

	if (strcmp(result,"egzloxi|ixw]dkSe]dzSzccShejSi^3q") == 0)
		puts("Congrats, that's right! Enter your input as the flag!");
	else
		puts("Oops, that wasn't right. Please try again!");

	return 0;

}
