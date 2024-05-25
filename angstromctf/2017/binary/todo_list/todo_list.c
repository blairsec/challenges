#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <dirent.h>

/* Maximum length of a username, password, and list item */
#define USER_LENGTH		16
#define PASS_LENGTH		32
#define ITEM_LENGTH		64

char user[USER_LENGTH];
char prog_dir[64];

static char *readline(char *buffer, int len, FILE *fp)
{
	if (fgets(buffer, len, fp) == NULL) return NULL;
	buffer[strcspn(buffer, "\n")] = 0;

	if (buffer[0] == 0) return NULL;
	else return buffer;
}

static void writeline(char *buffer, int len, FILE *fp)
{
	int newline_idx = strcspn(buffer, "\0");
	if (newline_idx == len) newline_idx = len - 1;

	buffer[newline_idx] = '\n';
	fwrite(buffer, newline_idx + 1, 1, fp);
}

static bool valid_string(char *str)
{
	for (char *s = str; *s != 0; s++) if (*s == '/' || *s == '.') return false;
	return true;
}

static char *read_list_name(char *buffer)
{
	printf("Enter the name of the list: ");
	readline(buffer, 16, stdin);

	if (!valid_string(buffer))
	{
		printf("Invalid character in list name\n");
		return NULL;
	}

	return buffer;
}

void create_list()
{
	char list_name[16];
	if (!read_list_name(list_name)) return;

	FILE *fp = fopen(list_name, "w");
	if (!fp)
	{
		printf("Error creating list\n");
		return;
	}

	char item[ITEM_LENGTH];
	while (readline(item, ITEM_LENGTH, stdin))
	{
		writeline(item, ITEM_LENGTH, fp);
	}

	fclose(fp);
}

void view_list()
{
	char list_name[16];
	if (!read_list_name(list_name)) return;

	FILE *fp = fopen(list_name, "r");
	if (!fp)
	{
		printf("Error opening list\n");
		return;
	}

	char item[ITEM_LENGTH];
	while (readline(item, ITEM_LENGTH, fp))
	{
		printf(item);
		printf("\n");
	}

	fclose(fp);
}

void addto_list()
{
	char list_name[16];
	if (!read_list_name(list_name)) return;

	FILE *fp = fopen(list_name, "a");
	if (!fp)
	{
		printf("Error opening list\n");
		return;
	}

	char item[ITEM_LENGTH];
	while (readline(item, ITEM_LENGTH, stdin))
	{
		writeline(item, ITEM_LENGTH, fp);
	}

	fclose(fp);
}

void delete_list()
{
	char list_name[16];
	if (!read_list_name(list_name)) return;

	if (unlink(list_name) == -1) printf("Error deleting list\n");
}

void show_lists()
{
	struct dirent *entry;
	DIR *dp;

	dp = opendir(".");
	if (!dp)
	{
		printf("Error opening lists directory\n");
		return;
	}

	while(entry = readdir(dp))
	{
		if (!valid_string(entry->d_name)) continue;
		puts(entry->d_name);
	}

	closedir(dp);
}

void change_password()
{
	printf("Enter a password: ");
	char passwd[PASS_LENGTH];
	memset(passwd, 0, PASS_LENGTH);
	readline(passwd, PASS_LENGTH, stdin);

	FILE *fp = fopen(".password", "w");
	fwrite(passwd, PASS_LENGTH, 1, fp);
	fclose(fp);
}

void login_user()
{
	chdir(prog_dir);

	bool logged_in = false;
	int fails = 0;
	do
	{
		printf("Enter username: ");
		readline(user, USER_LENGTH, stdin);

		if (!valid_string(user))
		{
			printf("Invalid character in username\n");
			fails++;
			continue;
		}

		int status = mkdir(user, S_IRWXU);
		if (status == 0)
		{
			chdir(user);
			change_password();
			printf("New user %s created\n", user);
			logged_in = true;
		}
		else if (status == -1 && errno == EEXIST)
		{
			chdir(user);
			
			printf("Enter password: ");
			char given_passwd[PASS_LENGTH];
			memset(given_passwd, 0, PASS_LENGTH);
			readline(given_passwd, PASS_LENGTH, stdin);

			FILE *fp = fopen(".password", "r");
			char real_passwd[PASS_LENGTH];
			memset(real_passwd, 0, PASS_LENGTH);
			fread(real_passwd, PASS_LENGTH, 1, fp);
			fclose(fp);

			if (strcmp(given_passwd, real_passwd) != 0)
			{
				printf("Invalid password\n");
				chdir("..");
				fails++;
				continue;
			}
			logged_in = true;
		}
		else
		{
			printf("Failed to create user directory\n");
			fails++;
		}
	} while(!logged_in && fails < 5);

	if (!logged_in) 
	{
		printf("Maximum number of failed logins exceeded\n");
		exit(-1);
	}
}

void print_help()
{
	printf("c - Create a new list\n");
	printf("v - View the contents of a list\n");
	printf("a - Add to an existing list\n");
	printf("d - Delete a list\n");
	printf("s - Show all the existing lists\n");
	printf("p - Change the user's password\n");
	printf("l - Login as a different user\n");
	printf("h - Print this very menu\n");
	printf("x - Exit the program\n\n");
}

void main_loop()
{
	printf("> ");

	char cmd[4];
	while (readline(cmd, 3, stdin))
	{
		switch (cmd[0])
		{
		case 'c':
			create_list();
			break;
		case 'v':
			view_list();
			break;
		case 'a':
			addto_list();
			break;
		case 'd':
			delete_list();
			break;
		case 's':
			show_lists();
			break;
		case 'p':
			change_password();
			break;
		case 'l':
			login_user();
			break;
		case 'h':
			print_help();
			break;
		case 'x':
			exit(0);
			break;
		default:
			break;
		}

		printf("> ");
	}
}

int main(int argc, char **argv)
{
	setvbuf(stdin, NULL, _IONBF, 0);
	setvbuf(stdout, NULL, _IONBF, 0);

	printf("Welcome to Noah's ListKeeper Pro!\n");
	printf("Keep your todo lists safely online and never worry about them again!\n");
	printf("Access them from your computer, phone, tablet, game console, car dashboard, or smart fridge!\n\n");

	printf("Let's start by getting you logged in\n");
	getcwd(prog_dir, 64);
	login_user();

	printf("Welcome, %s! Here are the commands you can use: \n", user);
	print_help();
	main_loop();
}
