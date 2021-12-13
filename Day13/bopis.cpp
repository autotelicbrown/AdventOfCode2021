#include <cstdio>
#include <set>
#include <tuple>
#include <algorithm>

#define N_POINTS 793 
#define N_FOLDS 1

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

  int ans = (int) dots.size();

  printf("Answer = %d\n", ans);

  return 0;
}
