#include <stdio.h>

#define INPUT_SIZE 10

int grid[INPUT_SIZE][INPUT_SIZE];

void visit(int x0, int y0, int *n) {
  if (grid[x0][y0] < 0)
    return;

  grid[x0][y0] += 1;
  if (grid[x0][y0] < 10)
    return;

  int adjx[8] = { -1, 0, 1, -1, 1, -1, 0, 1};
  int adjy[8] = { -1, -1, -1, 0, 0, 1, 1, 1};

  *n += 1;
  grid[x0][y0] = -1;
  for (int i = 0; i < 8; ++i) {
    int x = x0 + adjx[i];
    int y = y0 + adjy[i];
    if (x < 0 || x >= INPUT_SIZE || y < 0 || y >= INPUT_SIZE)
      continue;

    visit(x, y, n);
  }

  return;
}

int main(int argc, char *argv[])
{
  char buf[INPUT_SIZE+1];
  int nFlashes = 0;
  int ans;

  for (int i = 0; i < INPUT_SIZE; ++i) {
    scanf("%s", buf);
    for (int j = 0; j < INPUT_SIZE; ++j)
      grid[i][j] = buf[j] - '0';
  }

  int nFlashesPrev = 0;
  int iter = 1;
  while (1) {
    for (int i = 0; i < INPUT_SIZE; ++i)
      for (int j = 0; j < INPUT_SIZE; ++j)
        visit(i, j, &nFlashes);

    for (int i = 0; i < INPUT_SIZE; ++i)
      for (int j = 0; j < INPUT_SIZE; ++j)
        if (grid[i][j] < 0)
          grid[i][j] = 0;

    if (nFlashes - nFlashesPrev == INPUT_SIZE * INPUT_SIZE) {
      ans = iter;
      break;
    }

    nFlashesPrev = nFlashes;
    ++iter;
  }

  printf("Answer = %d\n", ans);

  return 0;
}

