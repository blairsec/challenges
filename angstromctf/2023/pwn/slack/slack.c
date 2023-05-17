#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <time.h>

char * messages[8] = {
    "Hi there! Just wanted to remind you that your weekly team meeting starts in 10 minutes.",
    "Your colleague John has just joined the channel! Say hi and welcome him to the conversation.",
    "Great job on completing your latest project milestone! Keep up the good work.",
    "Don't forget to fill out your time sheet for the week by 5 pm today.",
    "Looks like there's an issue with the server. Our IT team is currently working on a fix and we'll keep you updated.",
    "It's Jane's birthday today! Let's all wish her a happy birthday.",
    "Just a heads up - the office will be closed on Friday for a company-wide retreat.",
    "You've been added to the project planning channel. Start collaborating with your team and get the ball rolling!"
};

void main(){
    setbuf(stdout, NULL);
    setbuf(stdin, NULL);
    gid_t gid = getegid();
    setresgid(gid, gid, gid);

    struct tm* tm_info;

    time_t timer;

    char buffer[30];

    char input[30];

    puts("Welcome to slack (not to be confused with the popular chat service Slack)!");
    
    timer = time(NULL);
    tm_info = localtime(&timer);

    srand(time(NULL));

    for(int i = 0; i < 3; ++i){
        strftime(buffer, 26, "%Y-%m-%d %H:%M:%S", tm_info);
        printf("%s -- slack Bot:  %s\n", buffer, messages[rand() % 8]);

        printf("Your message (to increase character limit, pay $99 to upgrade to Professional): ");
        fgets(input, 14, stdin);

        tm_info = localtime(&timer);

        strftime(buffer, 26, "%Y-%m-%d %H:%M:%S", tm_info);
        printf("%s -- You: ", buffer);
        printf(input);
        printf("\n");
    }
}