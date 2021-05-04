#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <stdio.h>

char *
unstrfry (char *string, int32_t seed)
{
  int init = 0;
  struct random_data rdata = { 0 };
  if (!init)
    {
      char state[32] = { 0 };
      rdata.state = NULL;
      initstate_r (seed,
                     state, sizeof (state), &rdata);
      init = 1;
    }
  size_t len = strlen (string);
  int32_t nums[len - 1];
  for (int i = 0; i < len - 1; i++) random_r(&rdata, &(nums[i]));
  if (len > 0)
    for (size_t i = len - 2; i < (size_t)-1; --i)
      {
        int32_t j;
        j = nums[i];
        j = j % (len - i) + i;
        char c = string[i];
        string[i] = string[j];
        string[j] = c;
      }
  return string;
}

int main() {
  time_t s = 0;
  char string[50];
  scanf("%d\n", &s);
  scanf("%s", &string);
  strtok(string, "\n");
  for (pid_t i = 0; i < 1000000; i++) {
    char b[50];
    strcpy(&b, &string);
    printf("%s\n", unstrfry(b, s^i));
  }
}
