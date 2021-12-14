#include <cstdio>
#include <cstring>
#include <vector>
#include <map>
#include <algorithm>

const int N_TYPES = 10;
const int N_ITERS = 10;

int main(int argc, char *argv[])
{
  std::map<char, int> map;
  char input[32];
  char buf[3];

  scanf("%s ", input);

  // Mapping edges from the adjacency table to relevant info
  std::vector<int> current(N_TYPES * N_TYPES, 0);
  std::vector<int> tmp(N_TYPES * N_TYPES, 0);
  std::vector<int> byproduct1(N_TYPES * N_TYPES, -1);
  std::vector<int> byproduct2(N_TYPES * N_TYPES, -1);

  while (scanf("%c%c -> %c ", &(buf[0]), &(buf[1]), &(buf[2])) != EOF) {
    for (int i = 0; i < 3; ++i)
      if (map.find(buf[i]) == map.end())
        map.insert({buf[i], map.size()});

    // AB -> C means AB -> AC + CB
    int idx = map[buf[0]] + N_TYPES * map[buf[1]];
    byproduct1[idx] = map[buf[0]] + N_TYPES * map[buf[2]];
    byproduct2[idx] = map[buf[2]] + N_TYPES * map[buf[1]];
  }

  int len = strlen(input) - 1;
  for (int i = 0; i < len; ++i) {
    int idx = map[input[i]] + N_TYPES * map[input[i+1]];
    current[idx] += 1;
  }

  for (int iter = 0; iter < N_ITERS; ++iter) {
    for (int i = 0; i < N_TYPES; ++i) {
      int offset = i * N_TYPES;
      for (int j = 0; j < N_TYPES; ++j) {
        int idx = j + offset;
        tmp[byproduct1[idx]] += current[idx];
        tmp[byproduct2[idx]] += current[idx];
        current[idx] = 0;
      }
    }
    std::swap(current, tmp);
  }

  // Recover number of elements from the edge counts
  std::vector<int> count(N_TYPES, 0);
  for (int i = 0; i < N_TYPES; ++i) {
    int offset = i * N_TYPES;
    for (int j = 0; j < N_TYPES; ++j) {
      int idx = j + offset;
      count[i] += current[idx];
      count[j] += current[idx];
    }
  }
  count[map[input[0]]] += 1;
  count[map[input[strlen(input)-1]]] += 1;
  for (int &i: count)
    i /= 2;

  std::sort(count.begin(), count.end());
  int ans = count.back() - count.front();
  printf("Answer = %d\n", ans);

  return 0;
}
