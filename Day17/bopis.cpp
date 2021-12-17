#include <cstdio>
#include <cstdlib>

// Note: Part One was done without code

class Simulator {
  public:
    Simulator() {
      scanf("target area: x=%d..%d, y=%d..%d", &xLow, &xHigh, &yLow, &yHigh);
    }

    int FullSimulation() {
      if (yHigh >= 0) {
        printf("Solution assumes target area y is strictly negative\n");
        std::exit(-1);
      }
      if (xLow < 0) {
        printf("Solution assumes target area x is strictly non-negative\n");
        std::exit(-1);
      }

      int ans = 0;

      // If ySpeed > 0, the curve always reaches y = 0 again with speed (-ySpeed - 1)
      // For ySpeed < 0 at y = 0, anything lower than yLow will overshoot
      for (int ySpeed = yLow; ySpeed < -yLow; ++ySpeed) {
        // xSpeed never changes direction, so for non-negative target area it has to be non-negative
        // Additionally, if xSpeed > xHigh it will overshoot
        for (int xSpeed = 0; xSpeed <= xHigh; ++xSpeed) {
          if (Simulate(xSpeed, ySpeed))
            ans += 1;
        }
      }

      return ans;
    }

    bool Simulate(int xSpeed, int ySpeed) {
      int x = 0;
      int y = 0;
      while (1) {
        x += xSpeed;
        y += ySpeed;
        if (xSpeed > 0)
          xSpeed -= 1;
        ySpeed -= 1;

        if (y < yLow || x > xHigh)
          return false;

        if (y >= yLow && y <= yHigh && x >= xLow && x <= xHigh)
          return true;
      }
    }

  private:
    int xLow, xHigh, yLow, yHigh; 
};

int main(int argc, char *argv[])
{
  Simulator simulator;

  int ans = simulator.FullSimulation();

  printf("Answer = %d\n", ans);

  return 0;
}

