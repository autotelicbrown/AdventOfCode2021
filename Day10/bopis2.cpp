#include <cstdio>
#include <cstring>
#include <stack>
#include <vector>
#include <algorithm>

// ')' - '(' = 1
// '>' - '<' = 2
// ']' - '[' = 2
// '}' - '{' = 2
// All absolute differences across unmatched pairs are larger than 2

int main(int argc, char *argv[])
{
  char buf[256];
  std::vector<long long int> leaderboard;
  while (scanf("%s", buf) != EOF) {
    int len = strlen(buf);
    std::stack<char> stack;
    long long int score = 0;
    bool foundCriminal = false;

    for (int i = 0; i < len; ++i) {
      int diff;
      switch (buf[i]) {
        case '(':
        case '[':
        case '{':
        case '<':
          stack.push(buf[i]);
          break;
        case ')':
        case ']':
        case '}':
        case '>':
          diff = buf[i] - stack.top();
          if (diff < 0)
            diff = -diff;
          if (diff > 2)
            foundCriminal = true;
          stack.pop();
          break;
        default:
          printf("wooops\n");
          return -1;
      }
      if (foundCriminal)
        break;
    }

    if (foundCriminal)
      continue;

    while (!stack.empty()) {
      score *= 5;
      char c = stack.top();
      score += c == '(' ? 1 : c == '[' ? 2 : c == '{' ? 3 : 4;
      stack.pop();
    }

    if (score > 0)
      leaderboard.push_back(score);
  }

  std::sort(leaderboard.begin(), leaderboard.end());
  for (int i = 0; i < leaderboard.size(); ++i)
    printf("%lld ", leaderboard[i]);
  printf("\n");
  long long int ans = leaderboard[leaderboard.size() / 2];

  printf("Answer = %lld\n", ans);

  return 0;
}
