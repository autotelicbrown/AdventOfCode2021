#include <stdio.h>

#define N_BOARDS 100
#define N_DRAWS 100

int main(int argc, char *argv[])
{
  int draws[N_DRAWS];
  int boardsVal[N_BOARDS][25];
  int boardsFlag[N_BOARDS][25] = {0};
  int boardsColCount[N_BOARDS][5] = {0};
  int boardsRowCount[N_BOARDS][5] = {0};
  int winningFlag[N_BOARDS] = {0};
  int winningCount = 0;
  int winningBoard = -1;
  int winningDraw;

  for (int i = 0; i < N_DRAWS - 1; ++i)
    scanf("%d,", &(draws[i]));
  scanf("%d", &(draws[N_DRAWS-1]));

  for (int i = 0; i < N_BOARDS; ++i)
    for (int j = 0; j < 25; ++j)
      scanf("%d", &(boardsVal[i][j]));

  for (int i = 0; i < N_DRAWS; ++i) {
    for (int j = 0; j < N_BOARDS; ++j) {
      if (winningFlag[j])
        continue;

      for (int k = 0; k < 25; ++k) {
        if (boardsVal[j][k] == draws[i]) {
          // Fill board cell
          boardsFlag[j][k] = 1;

          // Update bingo progress
          int row = k / 5;
          int col = k % 5;
          ++boardsRowCount[j][row];
          ++boardsColCount[j][col];
          if (boardsRowCount[j][row] == 5 || boardsColCount[j][col] == 5) {
            winningFlag[j] = 1;
            ++winningCount;
            if (winningCount == N_BOARDS) {
              winningDraw = draws[i];
              winningBoard = j;
            }
            break;
          }
        }
      }
    }
    if (winningBoard >= 0)
      break;
  }

  int sumUnmarked = 0;
  for (int i = 0; i < 25; ++i)
    if (!boardsFlag[winningBoard][i])
      sumUnmarked += boardsVal[winningBoard][i];

  printf("Answer = %d * %d = %d\n", sumUnmarked, winningDraw, sumUnmarked * winningDraw);

  return 0;
}
