#include <stdio.h>

#define N_BITS 12

int main(int argc, char *argv[])
{
  char buf[N_BITS + 1];
  int count[N_BITS] = {0};
  while (scanf("%s", buf) != EOF)
    for (int i = 0; i < N_BITS; ++i)
      buf[i] == '1' ? ++count[i] : --count[i];

  int gamma = 0;
  int epsilon = 0;
  for (int i = 0; i < N_BITS; ++i) {
    if (count[i] > 0)
      gamma += (1 << (N_BITS - 1 - i));
    else
      epsilon += (1 << (N_BITS - 1 - i));
  }

  printf("Answer: %d\n", gamma * epsilon);

  return 0;
}
