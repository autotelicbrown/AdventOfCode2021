#include <stdio.h>

#define INPUT_SIZE 300
#define DAYS 256

int main(int argc, char *argv[])
{
  long long int nPerTimeLeft[9] = {0};

  int val;
  for (int i = 0; i < INPUT_SIZE; ++i) {
    scanf("%d,", &val);
    nPerTimeLeft[val] += 1;
  }

  for (int i = 0; i < DAYS; ++i) {
    long long int tmp = nPerTimeLeft[0];
    for (int j = 1; j < 9; ++j)
      nPerTimeLeft[j-1] = nPerTimeLeft[j];
    nPerTimeLeft[6] += tmp;
    nPerTimeLeft[8] = tmp;
  }

  long long int ans = 0;
  for (int i = 0; i < 9; ++i)
    ans += nPerTimeLeft[i];

  printf("Answer = %lld\n", ans);

  return 0;
}
