#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <queue>

#define INPUT_SIZE 100

int comp(const void * a, const void * b) {
  return (*((int *)a) - *((int *)b));
}

int main(int argc, char *argv[])
{
  int grid[INPUT_SIZE][INPUT_SIZE];
  int flag[INPUT_SIZE][INPUT_SIZE] = {0};
  char buf[INPUT_SIZE+1];
  int adjx[4] = { -1, 1, 0, 0 };
  int adjy[4] = { 0, 0, -1, 1 };
  std::queue<int> Qx;
  std::queue<int> Qy;
  int nBasins = 0;
  int basinSize[10000] = {0};

  for (int i = 0; i < INPUT_SIZE; ++i) {
    scanf("%s", buf);
    for (int j = 0; j < INPUT_SIZE; ++j) {
      grid[i][j] = buf[j] - '0';
      if (grid[i][j] == 9)
        flag[i][j] = 1;
    }
  }

  for (int i = 0; i < INPUT_SIZE; ++i) {
    for (int j = 0; j < INPUT_SIZE; ++j) {
      if (flag[i][j])
        continue;

      Qx.push(i);
      Qy.push(j);
      flag[i][j] = 1;

      while (!Qx.empty()) {
        int x0 = Qx.front();
        int y0 = Qy.front();
        Qx.pop();
        Qy.pop();

        ++basinSize[nBasins];

        for (int k = 0; k < 4; ++k) {
          int x = x0 + adjx[k];
          int y = y0 + adjy[k];

          if (x < 0 || x >= INPUT_SIZE || y < 0 || y >= INPUT_SIZE)
            continue;

          if (!flag[x][y]) {
            flag[x][y] = 1;
            Qx.push(x);
            Qy.push(y);
          }
        }
      }
      
      ++nBasins;
    }
  }

  qsort(basinSize, nBasins, sizeof(int), comp);

  int ans = basinSize[nBasins-1] * basinSize[nBasins-2] * basinSize[nBasins-3];

  printf("Answer = %d\n", ans);

  return 0;
}
