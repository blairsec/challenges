#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


typedef struct {
    char date[8];
    char content[20];
} Dream;

int MAX_DREAMS = 5;

Dream ** dreams = NULL;

void menu(){
    puts("----- MENU -----");
    puts("1. Sleep");
    puts("2. Sell");
    puts("3. Visit a psychiatrist");
}

void gosleep(){
    puts("3 doses of Ambien finally calms you down enough to sleep.");
    puts("Toss and turn all you want, your unconscious never loses its grip.");

    printf("In which page of your mind do you keep this dream? ");

    int idx = 0;
    scanf("%d", &idx);
    getchar();
    if(idx >= MAX_DREAMS || idx < 0 || dreams[idx] != NULL){
        puts("Invalid index!");
        exit(1);
    }


    Dream * d = malloc(sizeof(Dream));

    printf("What's the date (mm/dd/yy))? ");

    read(0, d->date, 8);

    d->date[strcspn(d->date, "\n")] = '\x00';

    printf("On %s, what did you dream about? ", d->date);
    
    read(0, d->content, 20);

    dreams[idx] = d;
}

void sell(){
    puts("You've come to sell your dreams.");
    printf("Which one are you trading in? ");
    
    int idx = 0;
    scanf("%d", &idx);
    getchar();
    if(idx >= MAX_DREAMS || idx < 0){
        puts("Out of bounds!");
        exit(1);
    }

    puts("You let it go. Suddenly you feel less burdened... less restrained... freed. At last.");
    free(dreams[idx]);

    puts("Your money? Pfft. Get out of here.");
}

void psychiatrist(){
    puts("Due to your HMO plan, you can only consult me to decipher your dream.");
    printf("What dream is giving you trouble? ");

    int idx = 0;
    scanf("%d", &idx);
    getchar();

    if (dreams[idx] == NULL){
        puts("Invalid dream!");
        exit(1);
    }

    printf("Hmm... I see. It looks like your dream is telling you that ");
    puts(dreams[idx]->content);

    puts("Due to the elusive nature of dreams, you now must dream it on a different day. Sorry, I don't make the rules. Or do I?");
    printf("New date: ");
    read(0, dreams[idx]->date, 8);
}


int main(){
    setbuf(stdout, NULL);
    gid_t gid = getegid();
    setresgid(gid, gid, gid);

    dreams = malloc(MAX_DREAMS * 8);

    puts("Welcome to the dream tracker.");
    puts("Sleep is where the deepest desires and most pushed-aside feelings of humankind are brought out.");
    puts("Confide a month of your time.");

    int choice = 0;

    while (1){
        menu();
        printf("> ");

        scanf("%d", &choice);
        getchar();

        switch(choice){
            case 1:
                gosleep();
                break;
            case 2:
                sell();
                break;
            case 3:
                psychiatrist();
                break;
            default:
                puts("Invalid input!");
                exit(1);
        }
    }

    return 0;
}