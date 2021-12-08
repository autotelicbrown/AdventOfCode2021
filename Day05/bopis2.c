#include <stdio.h>

#define DIM 1000

void swap(int *a, int *b)
{
  int aux;
  aux = *a;
  *a = *b;
  *b = aux;
}

int main(int argc, char *argv[])
{
  int x1, y1, x2, y2;
  int grid[DIM][DIM] = {0};

  while (scanf("%d,%d -> %d,%d", &x1, &y1, &x2, &y2) != EOF) {
    if (x1 == x2) {
      // Vertical line
      if (y1 > y2)
        swap(&y1, &y2);
      for (int i = y1; i <= y2; ++i)
        grid[x1][i] += 1;
    } else if (y1 == y2) {
      // horizontal line
      if (x1 > x2)
        swap(&x1, &x2);
      for (int i = x1; i <= x2; ++i)
        grid[i][y1] += 1;
    } else {
      // Diagonal line
      if (x1 > x2) {
        swap(&x1, &x2);
        swap(&y1, &y2);
      }
      int yIncrement = y1 < y2 ? 1 : -1;
      for (int i = x1; i <= x2; ++i) {
        grid[i][y1] += 1;
        y1 += yIncrement;
      }
    }
  }

  int ans = 0;
  for (int i = 0; i < DIM; ++i)
    for (int j = 0; j < DIM; ++j)
      if (grid[i][j] > 1)
        ++ans;

  /*
  for (int i = 0; i < DIM; ++i) {
    for (int j = 0; j < DIM; ++j)
      printf(" %d", grid[j][i]);
    printf("\n");
  }*/

  printf("Answer = %d\n", ans);

  return 0;
}

