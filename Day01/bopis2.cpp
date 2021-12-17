#include <cstdio>
#include <queue>

int main(int argc, char *argv[])
{
  const int SUM_RANGE = 3;
  std::queue<int> Q;
  int count = 0;
  int val;

  for (int i = 0; i < SUM_RANGE; ++i) {
    scanf("%d", &val);
    Q.push(val);
  }

  while (scanf("%d", &val) != EOF) {
    // Shared elements can be ignored, only new and oldest values matter
    if (val > Q.front())
      ++count;
    Q.pop();
    Q.push(val);
  }

  printf("Answer = %d\n", count);

  return 0;
}

