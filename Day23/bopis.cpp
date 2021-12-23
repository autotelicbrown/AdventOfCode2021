#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <vector>
#include <stack>
#include <algorithm>
#include <limits>

const static int MAP_XSIZE = 14;
const static int MAP_YSIZE = 5;
const static int MAP_FULLSIZE = MAP_XSIZE * MAP_YSIZE;

inline int MapPointToIdx(int x, int y) {
  return x + y * MAP_XSIZE;
}

inline void MapIdxToPoint(int idx, int &xOut, int &yOut) {
  xOut = idx % MAP_XSIZE;
  yOut = idx / MAP_XSIZE;
}

enum class Agent {
  None,
  Amber,
  Bronze,
  Copper,
  Desert
};

enum class Tile {
  Wall,
  CorridorStay,
  CorridorPassage,
  RoomAmber,
  RoomBronze,
  RoomCopper,
  RoomDesert
};

struct Path {
  int tgt;
  long long int cost;
};

bool IsRoomTile(Tile tile) {
  return (
      (tile == Tile::RoomAmber) ||
      (tile == Tile::RoomBronze) ||
      (tile == Tile::RoomCopper) ||
      (tile == Tile::RoomDesert));
}

bool IsCorrectRoom(Agent agent, Tile tile) {
  return (
      (agent == Agent::Amber && tile == Tile::RoomAmber) ||
      (agent == Agent::Bronze && tile == Tile::RoomBronze) ||
      (agent == Agent::Copper && tile == Tile::RoomCopper) ||
      (agent == Agent::Desert && tile == Tile::RoomDesert));
}

long long int GetAgentMovementCost(Agent agent) {
  switch(agent) {
    case Agent::Amber:
      return 1;
    case Agent::Bronze:
      return 10;
    case Agent::Copper:
      return 100;
    case Agent::Desert:
      return 1000;
    case Agent::None:
    default:
      printf("Error trying to evaluate cost of empty or invalid agent\n");
      std::exit(-1);
  }
}

class ProblemState {
  public:
    ProblemState(std::vector<Agent> agents = {}) :
      agents(std::move(agents)) {
        if (ProblemState::tiles.size() != MAP_FULLSIZE) {
          printf("Creating object before tiles have been initialized\n");
          std::exit(-1);
        }
      }

    static ProblemState ReadInitialState() {
      ProblemState problem = std::vector<Agent>(MAP_FULLSIZE, Agent::None);
      char c;
      int idx = 0;
      int x = 0;
      int y = 0;

      while ((c = getchar()) != EOF) {
        if (c == '\n') {
          x = 0;
          y += 1;
          continue;
        }

        assert(x < MAP_XSIZE);
        assert(y < MAP_YSIZE);
        int idx = x + y * MAP_XSIZE;
        switch(c) {
          case '.':
            if ((x >= 3) && (x <= 9) && (x % 2 == 1))
              tiles[idx] = Tile::CorridorPassage;
            else
              tiles[idx] = Tile::CorridorStay;
            break;
          case 'A':
          case 'B':
          case 'C':
          case 'D':
            switch(x) {
              case 3:
                tiles[idx] = Tile::RoomAmber;
                break;
              case 5:
                tiles[idx] = Tile::RoomBronze;
                break;
              case 7:
                tiles[idx] = Tile::RoomCopper;
                break;
              case 9:
                tiles[idx] = Tile::RoomDesert;
                break;
              default:
                printf("Problem reading room tile type\n");
                std::exit(-1);
            }

            switch(c) {
              case 'A':
                problem.agents[idx] = Agent::Amber;
                break;
              case 'B':
                problem.agents[idx] = Agent::Bronze;
                break;
              case 'C':
                problem.agents[idx] = Agent::Copper;
                break;
              case 'D':
                problem.agents[idx] = Agent::Desert;
                break;
              default:
                printf("Problem reading agent type\n");
                std::exit(-1);
            }
          default:
            break;
        }
        x += 1;
      }

      return std::move(problem);
    }

    void PrintState() const {
      for (int y = 0; y < MAP_YSIZE; ++y) {
        for (int x = 0; x < MAP_XSIZE; ++x) {
          int idx = x + y * MAP_XSIZE;
          if (agents[idx] != Agent::None) {
            switch(agents[idx]) {
              case Agent::Amber:
                printf("A");
                break;
              case Agent::Bronze:
                printf("B");
                break;
              case Agent::Copper:
                printf("C");
                break;
              case Agent::Desert:
                printf("D");
                break;
              default:
                printf("?");
                break;
            }
          } else {
            if (tiles[idx] == Tile::Wall)
              printf("#");
            else
              printf(".");
          }
        }
        printf("\n");
      }
    }

    bool IsAgentDone(int idx) const {
      int x, y;
      MapIdxToPoint(idx, x, y);
      return IsAgentDone(x, y, idx);
    }

    bool IsAgentDone(int x, int y) const {
      return IsAgentDone(x, y, MapPointToIdx(x, y));
    }

    bool IsAgentDone(int x, int y, int idx) const {
      assert(agents[idx] != Agent::None);
      bool tmp = IsCorrectRoom(agents[idx], tiles[idx]);
      if (y == 2)
        return (tmp && IsAgentDone(x, y + 1));
      return tmp;
    }

    std::vector<Path> GetMovementOptions(int root) const {
      int adjx[4] = {-1, 1, 0, 0};
      int adjy[4] = {0, 0, -1, 1};
      int x, y;
      std::vector<Path> res;

      // Simple search to find reachable tiles
      std::stack<Path> Q;
      std::vector<int> color(MAP_FULLSIZE, 0);
      color[root] = 1;
      Q.push({root, 0});
      while (!Q.empty()) {
        Path p = Q.top();
        Q.pop();

        // Add to result only if it's a valid final position
        if ((tiles[root] != Tile::CorridorStay) && (tiles[p.tgt] == Tile::CorridorStay)) {
          res.push_back(p);
        } else if (IsRoomTile(tiles[p.tgt]) && IsCorrectRoom(agents[root], tiles[p.tgt])) {
          MapIdxToPoint(p.tgt, x, y);
          int idx = MapPointToIdx(x, y + 1);
          if (y == 3 || IsCorrectRoom(agents[idx], tiles[idx]))
            res.push_back(p);
        }

        MapIdxToPoint(p.tgt, x, y);
        for (int i = 0; i < 4; ++i) {
          int dx = x + adjx[i];
          int dy = y + adjy[i];
          int idx = MapPointToIdx(dx, dy);
          if ((color[idx] > 0) || (tiles[idx] == Tile::Wall) || (agents[idx] != Agent::None))
            continue;
          color[idx] = 1;
          Q.push({idx, p.cost + GetAgentMovementCost(agents[root])});
        }
      }

      return res;
    }

    long long int GetLowerBoundCostEstimate() const {
      long long int estimate = 0;
      for (int i = 0; i < MAP_FULLSIZE; ++i) { 
        if (agents[i] == Agent::None || IsAgentDone(i))
          continue;

        int x, y;
        MapIdxToPoint(i, x, y);
        int dx = agents[i] == Agent::Amber ? 3 : 
          agents[i] == Agent::Bronze ? 5 :
          agents[i] == Agent::Copper ? 7 : 9;
        int dy = 2;
        if (x > dx)
          std::swap(x, dx);
        if (y > dy)
          std::swap(y, dy);
        estimate += ((dx - x) + (dy - y)) * GetAgentMovementCost(agents[i]);
      }
      return estimate;
    }

    long long int GetOptimalCost(long long int stateCost = 0, 
        long long int knownBestCost = std::numeric_limits<long long int>::max()) const {
      bool allAgentsDone = true;

      if (knownBestCost < std::numeric_limits<long long int>::max()) {
        long long int lowerBoundCostEstimate = GetLowerBoundCostEstimate();
        if (stateCost + lowerBoundCostEstimate >= knownBestCost)
          return knownBestCost;
      }

      for (int i = 0; i < MAP_FULLSIZE; ++i) {
        if (agents[i] == Agent::None || IsAgentDone(i))
          continue;

        allAgentsDone = false;
        std::vector<Path> moves = GetMovementOptions(i);
        for (Path p : moves) {
          assert(i != p.tgt);

          if (stateCost + p.cost >= knownBestCost)
            continue;

          std::vector<Agent> agentsAfterMove(agents);
          std::swap(agentsAfterMove[i], agentsAfterMove[p.tgt]);
          ProblemState subProblem(agentsAfterMove);
          long long int subProblemBestCost = subProblem.GetOptimalCost(stateCost + p.cost, knownBestCost);
          knownBestCost = std::min(knownBestCost, subProblemBestCost);
        }
      }

      if (allAgentsDone) {
        printf("Reached solution with cost %lld\n", stateCost);
        return stateCost;
      }
      return knownBestCost;
    }

  private:
    static std::vector<Tile> tiles;
    std::vector<Agent> agents;
};

std::vector<Tile> ProblemState::tiles{MAP_FULLSIZE, Tile::Wall};

int main(int argc, char *argv[])
{
  ProblemState problem = ProblemState::ReadInitialState();
  long long int ans = problem.GetOptimalCost();
  printf("Answer = %lld\n", ans);
  return 0;
}

