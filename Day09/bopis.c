#include <stdio.h>
#include <stdbool.h>

#define INPUT_SIZE 100

int main(int argc, char *argv[])
{
  int grid[INPUT_SIZE][INPUT_SIZE];
  char buf[INPUT_SIZE+1];
  int adjx[4] = { -1, 1, 0, 0 };
  int adjy[4] = { 0, 0, -1, 1 };
  int ans = 0;

  for (int i = 0; i < INPUT_SIZE; ++i) {
    scanf("%s", buf);
    for (int j = 0; j < INPUT_SIZE; ++j)
      grid[i][j] = buf[j] - '0';
  }

  for (int i = 0; i < INPUT_SIZE; ++i) {
    for (int j = 0; j < INPUT_SIZE; ++j) {
      bool isMinimal = true;
      for (int k = 0; k < 4; ++k) {
        int x = i + adjx[k];
        int y = j + adjy[k];

        if (x < 0 || x >= INPUT_SIZE || y < 0 || y >= INPUT_SIZE)
          continue;

        if (grid[i][j] >= grid[x][y]) {
          isMinimal = false;
          break;
        }
      }

      if (isMinimal)
        ans += grid[i][j] + 1;
    }
  }

  printf("Answer = %d\n", ans);

  return 0;
}
