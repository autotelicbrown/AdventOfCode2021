#include <stdio.h>
#include <stdlib.h>

#define INPUT_SIZE 1000

int comp(const void * a, const void * b) {
  return (*((int *)a) - *((int *)b));
}

int main(int argc, char *argv[])
{
  int crabs[INPUT_SIZE];
  for (int i = 0; i < INPUT_SIZE; ++i)
    scanf("%d,", &(crabs[i]));

  qsort(crabs, INPUT_SIZE, sizeof(int), comp);
  
  int line = crabs[INPUT_SIZE / 2];

  int ans = 0;
  for (int i = 0; i < INPUT_SIZE; ++i) {
    int dist = crabs[i] - line;
    ans += dist < 0 ? -dist : dist;
  }

  printf("Answer = %d\n", ans);

  return 0;
}

