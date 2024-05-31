#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_SIZE 200

typedef struct {
  size_t size;
  char *themes[];
} themelist;

typedef struct {
  char *name;
  char *passw;
  themelist *themes;
} user;

user *userlist[MAX_SIZE] = {0};
user *cur_user = NULL;
int user_index = 0;

void create_admin() {
  user *admin_user = malloc(sizeof(user));
  admin_user->name = "clammuffinhadabosh";

  admin_user->passw = (char *)malloc(128);

  int fp = open("/dev/urandom", O_RDONLY);

  char charset[] = "0123456789"
                   "abcdefghijklmnopqrstuvwxyz"
                   "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

  int chr;
  for (int i = 0; i < 128; i++) {
    read(fp, &chr, sizeof(int));
    admin_user->passw[i] = charset[chr % (sizeof(charset))];
  }
  close(fp);

  admin_user->themes =
      (themelist *)malloc(sizeof(themelist) + sizeof(char *) * 1);
  admin_user->themes->size = 1;
  admin_user->themes->themes[0] = "Jujutsu Kaisen";

  userlist[0] = admin_user;
}

int login() {
  printf("-- Log in -- \n");
  printf("Username: ");
  char name[300];
  fgets(name, 300, stdin);
  name[strcspn(name, "\n")] = '\0';

  printf("Enter your password: ");
  char passw[300];
  fgets(passw, 300, stdin);
  passw[strcspn(passw, "\n")] = '\0';

  for (int i = 0; i < MAX_SIZE; i++) {
    if (userlist[i] != NULL && strcmp(name, userlist[i]->name) == 0 &&
        strcmp(passw, userlist[i]->passw) == 0) {
      cur_user = userlist[i];
      user_index = i;
      return 0;
    }
  }

  printf("There was a problem logging in. \n");
  return 1;
}

int create_user() {

  printf("-- Create new user -- \n");

  printf("Username: ");
  char name[300];
  fgets(name, sizeof(name), stdin);
  name[strcspn(name, "\n")] = '\0';

  for (int i = 0; i < MAX_SIZE; i++) {
    if (userlist[i] != NULL && strcmp(name, userlist[i]->name) == 0) {
      printf("Cannot have duplicate names. \n");
      return 1;
    }
  }

  printf("Enter your password: ");
  char passw[300];
  fgets(passw, sizeof(passw), stdin);
  passw[strcspn(passw, "\n")] = '\0';

  printf("How many themes would you like? ");
  char inp[32];
  fgets(inp, 32, stdin);
  int themenum = abs(atoi(inp));
  themelist *thm_list = malloc(sizeof(themelist) + sizeof(char *) * themenum);

  user *new_user = malloc(sizeof(user));
  new_user->name = malloc(strlen(name) + 1);
  strcpy(new_user->name, name);

  new_user->passw = malloc(strlen(passw) + 1);
  strcpy(new_user->passw, passw);

  new_user->themes = thm_list;
  new_user->themes->size = themenum;

  int ind = 0;
  while (userlist[ind] != NULL) {
    ind += 1;
  }

  user_index = ind;
  userlist[user_index] = new_user;
  cur_user = new_user;
  return 0;
}

int main() {

  setbuf(stdin, NULL);
  setbuf(stdout, NULL);

  printf("Welcome to the Angstrom Theme Submitter! \n");
  printf("Submit your theme ideas for next year's challenges here. \n");

  create_admin();

  char inp[32];

  while (true) {
    if (cur_user == NULL) {
      printf("--- OPTIONS --- \n");
      printf("1. Register\n");
      printf("2. Log in\n");
      printf("3. Exit\n");
      printf("> ");
      fgets(inp, 32, stdin);
      int choice = atoi(inp);
      if (choice == 1) {
        create_user();
      } else if (choice == 2) {
        login();
      } else if (choice == 3) {
        return 0;
      } else {
        printf("Invalid choice. \n");
      }
    } else {
      printf("--- OPTIONS --- \n");
      printf(" You have [%ld] theme ideas.\n", cur_user->themes->size);
      printf("1. Edit a theme\n");
      printf("2. View a theme\n");
      printf("3. Submit themes for review\n");
      printf("4. Log out\n");
      printf("> ");
      fgets(inp, 32, stdin);
      int choice = atoi(inp);
      if (choice == 1) {
        printf("Which theme would you like to edit? ");
        fgets(inp, 32, stdin);
        int ind = atoi(inp);
        if (0 <= ind && ind < cur_user->themes->size) {
          printf("Enter a theme idea: ");
          if (cur_user->themes->themes[ind] != NULL) {
            gets(cur_user->themes->themes[ind]);
          } else {
            char *theme_idea = (char *)malloc(32);
            gets(theme_idea);
            cur_user->themes->themes[ind] = theme_idea;
          }
        } else {
          printf("Not a valid index.\n");
        }
      } else if (choice == 2) {
        printf("Which theme would you like to view? ");
        fgets(inp, 32, stdin);
        int ind = atoi(inp);
        if (0 <= ind && ind < cur_user->themes->size) {
          if (cur_user->themes->themes[ind] != NULL) {
            printf("%s\n", cur_user->themes->themes[ind]);
          }
        }
      } else if (choice == 3) {
        printf("Unimplemented.\n");
      } else if (choice == 4) {
        cur_user = NULL;
      } else {
        printf("Invalid choice. \n");
      }
    }
  }
}
