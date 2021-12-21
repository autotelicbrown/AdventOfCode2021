#include <stdio.h>
#include <assert.h>

#define MAX_TURNS 10
#define NUM_PLAYERS 2
#define NUM_POSITIONS 10
#define NUM_SCORES 21
#define MAX_ROLL 9

int main(int argc, char *argv[])
{
  // Which turn X Whose turn X Player 1 pos X Player 2 pos X Player 1 score X Player 2 score
  long long int mx[MAX_TURNS][NUM_PLAYERS][NUM_POSITIONS][NUM_POSITIONS][NUM_SCORES][NUM_SCORES] = {0};
  long long int statesPerRoll[MAX_ROLL+1] = {0, 0, 0, 1, 3, 6, 7, 6, 3, 1};

  int playerStartPos[2];
  for (int player = 0; player < NUM_PLAYERS; ++player) {
    int pos;
    scanf("Player %*d starting position: %d ", &pos);
    playerStartPos[player] = pos % 10;
  }

  // Dynamic programming
  mx[0][0][playerStartPos[0]][playerStartPos[1]][0][0] = 1;
  long long int score[2] = {0};
  for (int turn = 0; turn < MAX_TURNS; ++turn) {
    for (int player = 0; player < NUM_PLAYERS; ++player) {
      for (int posP1 = 0; posP1 < NUM_POSITIONS; ++posP1) {
        for (int posP2 = 0; posP2 < NUM_POSITIONS; ++posP2) {
          for (int scoreP1 = 0; scoreP1 < NUM_SCORES; ++scoreP1) {
            for (int scoreP2 = 0; scoreP2 < NUM_SCORES; ++scoreP2) {

              long long int curStates = mx[turn][player][posP1][posP2][scoreP1][scoreP2];
              if (curStates == 0)
                continue;

              for (int roll = 3; roll <= MAX_ROLL; ++roll) {
                long long int newStates = curStates * statesPerRoll[roll];
                int newPos, newScore;
                if (player == 0) {
                  newPos = ((posP1 + roll) % 10);
                  newScore = scoreP1 + (newPos > 0 ? newPos : 10);
                  if (newScore >= NUM_SCORES)
                    score[0] += newStates;
                  else
                    mx[turn][1][newPos][posP2][newScore][scoreP2] += newStates;
                } else {
                  newPos = ((posP2 + roll) % 10);
                  newScore = scoreP2 + (newPos > 0 ? newPos : 10);
                  if (newScore >= NUM_SCORES)
                    score[1] += newStates;
                  else {
                    assert(turn + 1 < MAX_TURNS);
                    mx[turn+1][0][posP1][newPos][scoreP1][newScore] += newStates;
                  }
                }
              }
            }
          }
        }
      }
    }
  }

  long long int ans = (score[0] > score[1] ? score[0] : score[1]);
  printf("Answer = %lld\n", ans);

  return 0;
}

