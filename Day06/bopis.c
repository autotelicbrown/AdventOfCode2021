#include <stdio.h>

#define INPUT_SIZE 300
#define DAYS 80
#define BUF_SIZE 1000000

int main(int argc, char *argv[])
{
  int nFish = INPUT_SIZE;
  int fishArr[BUF_SIZE];

  for (int i = 0; i < BUF_SIZE; ++i)
    fishArr[i] = 8;
  for (int i = 0; i < INPUT_SIZE - 1; ++i)
    scanf("%d,", &(fishArr[i]));
  scanf("%d", &(fishArr[INPUT_SIZE-1]));

  for (int i = 0; i < DAYS; ++i) {
    int dayFish = nFish;
    for (int j = 0; j < dayFish; ++j) {
      if (fishArr[j] == 0) {
        fishArr[j] = 6;
        ++nFish;
      } else {
        --fishArr[j];
      }
    }
  }

  printf("Answer = %d\n", nFish);

  return 0;
}
