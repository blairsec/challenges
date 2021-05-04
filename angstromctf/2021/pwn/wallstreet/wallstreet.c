#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

char user_buf[301];

char * pick_symbol_with_AI() {
    char * stonk = calloc(5, sizeof(char));

    int AI_symbol_len = (rand() % 4) + 1;

    for (int i = 0; i <= 4; i++) {
        if (i < AI_symbol_len) {
            stonk[i] = 'A' + (rand() % 26);
        } else {
            stonk[i] = '\0';
        }
    }

    return stonk;
}


int buy_stonks() {
    char * portfolio[sizeof(char *)*8];
    printf("Buying stonks...\n");
    
    for(int i = 0; i < 8; ++i){
        portfolio[i] = pick_symbol_with_AI();
    }
    
    printf("What stonk do you want to see?\n");    
    int idx = 0;
    char ** stonkidx = &portfolio;
    
    scanf("%d", &idx);
    stonkidx += idx;
    
    printf("%s\n", *stonkidx);
    
//     char * user_buf = calloc(301, sizeof(char));
    
    
    printf("What is your API token?\n");
    
    getchar();
    read(0, &user_buf, 300);
    
    int count = 0;
    for(int i = 0; i < 300; ++i){
        switch(user_buf[i]){
            case 'd':
            case 'i':
            case 'u':
            case 'o':
            case 'x':
            case 'X':
            case 'f':
            case 'F':
            case 'e':
            case 'E':
            case 'g':
            case 'G':
            case 'a':
            case 'A':
            case 'c':
            case 's':
            case 'p':
                if(count >= 1 || user_buf[i] != 'c'){
                    printf("Hey! Only one leak allowed!\n");
                    exit(0);
                } else {
                    count++;
                }
        }
    }
    
    printf("Buying stonks with token:\n");
    printf(user_buf);
    printf("\n");
    
    return 0;
}







int main(){
    setbuf(stdout, NULL);
    setbuf(stderr, NULL);
    
    srand(time(NULL));
    
    
    int resp = 0;
    printf("Welcome back to the trading app!\n\n");
    printf("What would you like to do?\n");
    printf("1) Buy some stonks!\n");

    scanf("%d", &resp);
    
    if (resp == 1){
        buy_stonks();
    }
    
    return 0;
}


