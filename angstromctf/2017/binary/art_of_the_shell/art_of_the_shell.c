#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void be_nice_to_people()
{
    gid_t gid = getegid();
    setresgid(gid, gid, gid);
}

void vuln(char *input)
{
	char buf[64];
	strcpy(buf, input);
}

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		printf("Usage: art_of_the_shell [str]\n");
		return 1;
	}

	be_nice_to_people();
	vuln(argv[1]);

	return 0;
}
