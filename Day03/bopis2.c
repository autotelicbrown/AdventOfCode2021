#include <stdio.h>

#define N_BITS 12
#define N_LINES 1000

int main(int argc, char *argv[])
{
  char buf[N_LINES][N_BITS + 1];
  int oxyBitCount;
  int oxyFlag[N_LINES] = {0};
  int oxy = 0;
  int co2BitCount;
  int co2Flag[N_LINES] = {0};
  int co2 = 0;
  int co2NumLeft = N_LINES;
  int co2LastLine = -1;

  for (int i = 0; i < N_LINES; ++i)
    scanf("%s", buf[i]);

  for (int j = 0; j < N_BITS; ++j) {
    oxyBitCount = co2BitCount = 0;

    // Get bit counts
    for (int i = 0; i < N_LINES; ++i) {
      if (!oxyFlag[i])
        oxyBitCount += (buf[i][j] == '1' ? 1 : -1);
      if (!co2Flag[i])
        co2BitCount += (buf[i][j] == '1' ? 1 : -1);
    }

    // Add column value to result
    if (oxyBitCount >= 0)
      oxy += (1 << (N_BITS - 1 - j));
    if (((co2NumLeft > 1) && (co2BitCount < 0)) || ((co2NumLeft == 1) && (buf[co2LastLine][j] == '1')))
      co2 += (1 << (N_BITS - 1 - j));

    // Filter based on bit criteria with flags
    for (int i = 0; i < N_LINES; ++i) {
      if (buf[i][j] == '0') {
        if (oxyBitCount >= 0)
          oxyFlag[i] = 1;
        if (co2NumLeft > 1 && !co2Flag[i] && co2BitCount < 0) {
          co2Flag[i] = 1;
          --co2NumLeft;
        }
      } else {
        if (oxyBitCount < 0)
          oxyFlag[i] = 1;
        if (co2NumLeft > 1 && !co2Flag[i] && co2BitCount >= 0) {
          co2Flag[i] = 1;
          --co2NumLeft;
        }
      }
    }

    if (co2NumLeft == 1 && co2LastLine < 0)
      for (int i = 0; i < N_LINES; ++i)
        if (!co2Flag[i]) {
          co2LastLine = i;
          break;
        }
  }

  printf("Answer: %d * %d = %d\n", oxy, co2, oxy * co2);

  return 0;
}

