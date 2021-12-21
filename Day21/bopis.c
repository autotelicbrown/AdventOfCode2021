#include <stdio.h>

#define NUM_PLAYERS 2
#define NUM_OUTCOMES 10

int main(int argc, char *argv[])
{
  int outcome[NUM_PLAYERS][NUM_OUTCOMES];
  for (int player = 0; player < NUM_PLAYERS; ++player) {
    int pos;
    scanf("Player %*d starting position: %d ", &pos);

    // Starting rolls are 1+2+3 = 6 and 4+5+6 = 15 % 10 = 5
    int roll = (player == 0 ? 6 : 5);

    // Player always loops back to starting position/roll after 10 rolls
    for (int i = 0; i < NUM_OUTCOMES; ++i) {
      pos = ((pos + roll) % 10);

      outcome[player][i] = pos;
      if (pos == 0)
        outcome[player][i] = 10;

      // Given a roll x + (x+1) + (x+2) = 3x + 3, the following
      //   roll is (x+6) + (x+7) + (x+8) = 3x + 21 so the effective
      //   difference is (+18 % 10 = +8)
      roll = ((roll + 8) % 10);
    }
  }

  int score[NUM_PLAYERS] = {0};
  int turn = 0;
  int player;
  do {
    turn += 1;
    player = ((turn + 1) % NUM_PLAYERS);
    score[player] += outcome[player][((turn - 1) / NUM_PLAYERS) % NUM_OUTCOMES];
  } while (score[player] < 1000);

  int loser = ((player + 1) % NUM_PLAYERS);
  int ans = (turn * 3) * score[loser];
  printf("Answer = %d\n", ans);

  return 0;
}

