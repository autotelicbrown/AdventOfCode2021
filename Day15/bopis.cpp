#include <cstdio>
#include <climits>
#include <queue>

static const int DIM = 100;
static const int IMG_SIZE = DIM * DIM;

int main(int argc, char *argv[])
{
  int img[IMG_SIZE];
  int cost[IMG_SIZE];
  int pred[IMG_SIZE];
  int color[IMG_SIZE] = {0};
  int adjx[4] = {-1, 1, 0, 0};
  int adjy[4] = {0, 0, -1, 1};

  for (int y = 0; y < DIM; ++y) {
    int offset = y * DIM;
    for (int x = 0; x < DIM; ++x) {
      int idx = x + offset;
      char c;
      scanf("%c", &c);
      img[idx] = c - '0';
      cost[idx] = INT_MAX;
      pred[idx] = idx;
    }
    scanf(" ");
  }

  auto cmp = [&cost](int a, int b) { return cost[a] > cost[b]; };
  std::priority_queue<int, std::vector<int>, decltype(cmp)> nodeHeap(cmp);
  cost[0] = 0;
  color[0] = 1;
  nodeHeap.push(0);
  while (!nodeHeap.empty()) {
    int p = nodeHeap.top();
    nodeHeap.pop();

    int x0 = p % DIM;
    int y0 = p / DIM;
    for (int i = 0; i < 4; ++i) {
      int x = x0 + adjx[i];
      int y = y0 + adjy[i];
      if (x < 0 || x >= DIM || y < 0 || y >= DIM)
        continue;

      int q = x + y * DIM;
      if (color[q])
        continue;

      cost[q] = cost[p] + img[q];
      pred[q] = p;
      color[q] = 1;
      nodeHeap.push(q);
    }
  }

  int ans = cost[IMG_SIZE - 1];
  printf("Answer: %d\n", ans);

  return 0;
}

