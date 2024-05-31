#include <stdio.h>
#include <string.h>
#include <limits.h>

int main() {
    setbuf(stdout, NULL);
    int trust_level = 0;
    int threshold = INT_MAX-1; // You'll never be trustworthy enough >:)

    char input[150];
    char flag[64]; // You'll never get this >:)
    const char statement[] = "I confirm that I am taking this exam between the dates 5/24/2024 and 5/27/2024. I will not disclose any information about any section of this exam.\n"; // Your only chance >:)

    int detrust;
    printf("How much should I not trust you? >:) ");
    scanf("%d", &detrust);

    fgets(input, sizeof(input), stdin); //throwaway
    if (detrust < 0){
        printf("Don't try to trick me into trusting you >:(\n");
    }
    else{
        trust_level -= detrust;
        if (trust_level == threshold){
            printf("What kind of cheating are you doing?\n");
            printf("You haven't even signed your statement yet!\n");
            printf("You are BANNED from all future AP exams!!!\n");
        }
        else{
            while (trust_level < threshold){
                printf("\nI don't trust you enough >:)\n");
                printf("Prove your trustworthyness by reciting the statement on the front cover of the Section I booklet >:)\n");

                fgets(input, sizeof(input), stdin);
                if (strcmp(input, statement) == 0) {
                    trust_level--; // When did I say that would make you more trustworthy? >:)
                }
            }
            FILE *fd = fopen("flag.txt", "r");
            fgets(flag, 64, fd);    

            printf("\nYou will now take the multiple-choice portion of the exam.\n");
            printf("You should have in front of you the multiple-choice booklet and your answer sheet. \n");
            printf("You will have %s minutes for this section. Open your Section I booklet and begin.\n", flag);
        }
    }

    return 0;
}
