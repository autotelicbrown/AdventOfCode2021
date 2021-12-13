#include <cstdio>
#include <set>
#include <tuple>
#include <algorithm>
#include <vector>

#define N_POINTS 793 
#define N_FOLDS 12

inline int abs(int a)
{
  return a * ((a > 0) - (a < 0));
}

typedef std::tuple<int, int> Point;

int main(int arg, char *argv[])
{
  std::set<Point> dots;
  for (int i = 0; i < N_POINTS; ++i) {
    int x, y;
    scanf("%d,%d", &x, &y);
    dots.insert({x, y});
  }

  for (int i = 0; i < N_FOLDS; ++i) {
    char axis;
    int fold;
    scanf(" fold along %c=%d", &axis, &fold);

    std::set<Point> tmpDots;
    for (Point p: dots) {
      int x, y;
      std::tie(x, y) = p;
      if (axis == 'x') {
        if (x == fold)
          continue;
        x = fold - abs(fold - x);
      } else {
        if (y == fold)
          continue;
        y = fold - abs(fold - y);
      }
      tmpDots.insert({x, y});
    }
    std::swap(dots, tmpDots);
  }

  int xSize = 0;
  int ySize = 0;
  for (Point p: dots) {
    int x, y;
    std::tie(x, y) = p;
    if (x + 1 > xSize)
      xSize = x + 1;
    if (y + 1 > ySize)
      ySize = y + 1;
  }

  std::vector<char> grid(xSize * ySize, '.');
  for (Point p: dots) {
    int x, y;
    std::tie(x, y) = p;
    grid[x + y * xSize] = '#';
  }

  for (int y = 0; y < ySize; ++y) {
    int offset = y * xSize;
    for (int x = 0; x < xSize; ++x)
      printf("%c", grid[x + offset]);
    printf("\n");
  }

  return 0;
}
