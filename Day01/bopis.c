#include<stdio.h>

int main(int argc, char *argv[])
{
  int prev, cur;
  int count = 0;

  scanf("%d", &prev);
  while (scanf("%d", &cur) != EOF) {
    if (cur > prev)
      ++count;
    prev = cur;
  }
  printf("Answer: %d\n", count);

  return 0;
}
