#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main(int argc, char *argv[]) {
	char *env = getenv("TERM");
	char term[256] = "TERM=";
	strncat(term, env, 250);
	setregid(getegid(), getegid());
	chdir("/problems/2021/snake/");
	execle("/problems/2021/snake/snake.py", "snake.py", (char *)NULL, (char *const []){term, NULL});
}
