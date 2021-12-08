#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define N_LINES 200

// true if a is in B
bool hasMatch(char a, char *B, int lenB)
{
  bool foundMatch = false;
  for (int i = 0; i < lenB; ++i) {
    if (a == B[i]) {
      foundMatch = true;
      break;
    }
  }

  return foundMatch;
}

// true if all chars in B are in A
bool isSubset(char *A, int lenA, char *B, int lenB)
{
  for (int i = 0; i < lenB; ++i)
    if (!hasMatch(B[i], A, lenA))
      return false;

  return true;
}

int main(int argc, char *argv[])
{
  int ans1 = 0;
  int ans2 = 0;

  for (int line = 0; line < N_LINES; ++line) {
    char signals[10][8] = {0};
    int numToSignal[10];
    int signalToNum[10];
    char display[4][8] = {0};

    for (int i = 0; i < 10; ++i) {
      scanf("%s", signals[i]);
      numToSignal[i] = -1;
    }
    scanf(" | ");
    for (int i = 0; i < 4; ++i)
      scanf("%s", display[i]);

    // Find trivial assignments
    for (int i = 0; i < 10; ++i) {
      switch (strlen(signals[i])) {
        case 2:
          numToSignal[1] = i;
          break;
        case 3:
          numToSignal[7] = i;
          break;
        case 4:
          numToSignal[4] = i;
          break;
        case 7:
          numToSignal[8] = i;
          break;
        default:
          break;
      }
    }


    // Identify 6 segment numbers
    for (int i = 0; i < 10; ++i) {
      if (strlen(signals[i]) == 6) {
        if (isSubset(signals[i], 6, signals[numToSignal[4]], 4)) {
          numToSignal[9] = i;
        } else if (!isSubset(signals[i], 6, signals[numToSignal[1]], 2)) {
          numToSignal[6] = i;
        } else {
          numToSignal[0] = i;
        }
      }
    }

    // Identify 5 segment numbers
    for (int i = 0; i < 10; ++i) {
      if (strlen(signals[i]) == 5) {
        if (isSubset(signals[i], 5, signals[numToSignal[1]], 2)) {
          numToSignal[3] = i;
        } else if (isSubset(signals[numToSignal[9]], 6, signals[i], 5)) {
          numToSignal[5] = i;
        } else {
          numToSignal[2] = i;
        }
      }
    }

    // Reverse indexing for convenience
    for (int i = 0; i < 10; ++i)
      signalToNum[numToSignal[i]] = i;

    // Convert display strings into actual value
    int displayVal[4];
    for (int i = 0; i < 4; ++i) {
      int len = strlen(display[i]);
      for (int j = 0; j < 10; ++j) {
        // Testing set equality
        if (len == strlen(signals[j]) && isSubset(display[i], len, signals[j], len)) {
          displayVal[i] = signalToNum[j];
          break;
        }
      }
    }

    int aux = 0;
    for (int i = 0; i < 4; ++i) {
      int c = displayVal[i];
      if (c == 1 || c == 4 || c == 7 || c == 8)
        ++ans1;
      aux = aux * 10 + c;
    }
    ans2 += aux;
  }

  printf("Answer 1 = %d\n", ans1);
  printf("Answer 2 = %d\n", ans2);

  return 0;
}

