#include <stdio.h>
#include <stdlib.h>

#define INPUT_SIZE 1000

int comp(const void * a, const void * b) {
  return (*((int *)a) - *((int *)b));
}

int calcCost(int L, int *arr, int len)
{
  int cost = 0;
  for (int i = 0; i < len; ++i) {
    int distL0 = arr[i] - L;
    if (distL0 < 0)
      distL0 = -distL0;
    cost += ((distL0 * (distL0+1)) / 2);
  }
  return cost;
}

int main(int argc, char *argv[])
{
  int crabs[INPUT_SIZE];
  for (int i = 0; i < INPUT_SIZE; ++i)
    scanf("%d,", &(crabs[i]));

  qsort(crabs, INPUT_SIZE, sizeof(int), comp);
  
  // floor of mean is a good starting guess (within +-1 of closed solution?)
  int mean = 0;
  for (int i = 0; i < INPUT_SIZE; ++i)
    mean += crabs[i];
  mean /= INPUT_SIZE;

  // Function is convex, so we can find solution by gradient descent
  int currentIdx = mean; 
  int currentCost = calcCost(currentIdx, crabs, INPUT_SIZE);
  int direction = 0;
  int costDownwards, costUpwards;
  while(1) {
    printf("Testing answer %d (cost = %d)\n", currentIdx, currentCost);

    if (direction <= 0)
      costDownwards = calcCost(currentIdx - 1, crabs, INPUT_SIZE);
    if (direction >= 0)
      costUpwards = calcCost(currentIdx + 1, crabs, INPUT_SIZE);

    if (costDownwards < currentCost) {
      direction = -1;
      currentIdx -= 1;
      currentCost = costDownwards;
    } else if (costUpwards < currentCost) {
      direction = 1;
      currentIdx += 1;
      currentCost = costUpwards;
    } else {
      break;
    }
  }
  
  printf("Answer = %d\n", currentCost);

  return 0;
}

