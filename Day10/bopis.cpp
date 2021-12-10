#include <cstdio>
#include <cstring>
#include <stack>

// ')' - '(' = 1
// '>' - '<' = 2
// ']' - '[' = 2
// '}' - '{' = 2
// All absolute differences across unmatched pairs are larger than 2

int main(int argc, char *argv[])
{
  char buf[256];
  int countCoxa[256] = {0};
  while (scanf("%s", buf) != EOF) {
    int len = strlen(buf);
    std::stack<char> stack;
    for (int i = 0; i < len; ++i) {
      bool foundCriminal = false;
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
          if (diff > 2) {
            printf("Expected pair for %c and found %c.\n", stack.top(), buf[i]);
            ++countCoxa[(int)buf[i]];
            foundCriminal = true;
          }
          stack.pop();
          break;
        default:
          printf("wooops\n");
          return -1;
      }
      if (foundCriminal)
        break;
    }
  }

  int ans = countCoxa[')'] * 3 + countCoxa[']'] * 57 +
    countCoxa['}'] * 1197 + countCoxa['>'] * 25137;

  printf("Answer = %d\n", ans);

  return 0;
}
