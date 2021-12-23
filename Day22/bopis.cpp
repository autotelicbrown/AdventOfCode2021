#include <cstdio>
#include <cassert>
#include <set>
#include <algorithm>

static const int BOUND = 50;

namespace cube {
  class BoundingCube {
    public:
      BoundingCube(int x1, int x2, int y1, int y2, int z1, int z2, int polarity = 1) :
        x1(x1), x2(x2), y1(y1), y2(y2), z1(z1), z2(z2), polarity(polarity) {
          assert(x1 <= x2);
          assert(y1 <= y2);
          assert(z1 <= z2);
          volume = (x2 - x1) * (y2 - y1) * (z2 - z1);
        }

      long long int GetVolume() const {
        return this->volume;
      }

      int GetPolarity() const {
        return this->polarity;
      }

      void SetPolarity(int newPolarity) {
        this->polarity = newPolarity;
      }

      void PrintCube() const {
        printf("Cube x = %d..%d, y = %d..%d, z = %d..%d, volume = %lld, polarity = %d\n",
            x1, x2, y1, y2, z1, z2, volume, polarity);
      }

      friend bool IsComplement(const BoundingCube &lhs, const BoundingCube &rhs);
      friend BoundingCube GetIntersection(const BoundingCube &lhs, const BoundingCube &rhs);

    private:
      int x1, x2;
      int y1, y2;
      int z1, z2;
      int polarity;
      long long int volume;
  };

  struct VolumeComparison {
    bool operator() (const BoundingCube &lhs, const BoundingCube &rhs) const {
      return lhs.GetVolume() < rhs.GetVolume();
    }
  };

  bool IsComplement(const BoundingCube &lhs, const BoundingCube &rhs) {
    return ((lhs.polarity != rhs.polarity) && 
      (lhs.x1 == rhs.x1) && (lhs.x2 == rhs.x2) &&
      (lhs.y1 == rhs.y1) && (lhs.y2 == rhs.y2) &&
      (lhs.z1 == rhs.z1) && (lhs.z2 == rhs.z2));
  }

  BoundingCube GetIntersection(const BoundingCube &lhs, const BoundingCube &rhs) {
    int x1 = std::max(lhs.x1, rhs.x1);
    int x2 = std::min(lhs.x2, rhs.x2);
    int y1 = std::max(lhs.y1, rhs.y1);
    int y2 = std::min(lhs.y2, rhs.y2);
    int z1 = std::max(lhs.z1, rhs.z1);
    int z2 = std::min(lhs.z2, rhs.z2);
    if (((x2 - x1) <= 0) || ((y2 - y1) <= 0) || ((z2 - z1) <= 0))
      return BoundingCube(0, 0, 0, 0, 0, 0);
    return BoundingCube(x1, x2, y1, y2, z1, z2);
  }
}

int main(int argc, char *argv[]) {
  std::multiset<cube::BoundingCube, cube::VolumeComparison> cubeSet;
  cube::BoundingCube validArea(-BOUND, BOUND + 1, -BOUND, BOUND + 1, -BOUND, BOUND + 1);

  char buf[4];
  int x1, x2, y1, y2, z1, z2;
  while (scanf("%s x=%d..%d,y=%d..%d,z=%d..%d ", buf, &x1, &x2, &y1, &y2, &z1, &z2) != EOF) {
    if (x1 > x2)
      std::swap(x1, x2);
    if (y1 > y2)
      std::swap(y1, y2);
    if (z1 > z2)
      std::swap(z1, z2);
    cube::BoundingCube bcAdded(x1, x2+1, y1, y2+1, z1, z2+1);
    bcAdded = cube::GetIntersection(bcAdded, validArea);
    if (buf[1] == 'f')
      bcAdded.SetPolarity(-1);

    if (bcAdded.GetVolume() <= 0)
      continue;

    std::multiset<cube::BoundingCube, cube::VolumeComparison> tmpSet;
    for (auto &bc : cubeSet) {
      cube::BoundingCube intersection = cube::GetIntersection(bc, bcAdded);

      if (intersection.GetVolume() <= 0)
        continue;

      if (bc.GetPolarity() == 1)
        intersection.SetPolarity(-1);
      tmpSet.insert(intersection);
    }

    if (bcAdded.GetPolarity() == 1)
      tmpSet.insert(bcAdded);

    for (auto &bc : tmpSet) {
      auto match = std::find_if(cubeSet.lower_bound(bc), cubeSet.upper_bound(bc),
          [&bc](const cube::BoundingCube &rhs ) { return cube::IsComplement(bc, rhs); });

      if (match != cubeSet.upper_bound(bc))
        cubeSet.erase(match);
      else
        cubeSet.insert(bc);
    }
  }

  long long int ans = 0;
  for (auto &bc : cubeSet)
    ans += (bc.GetVolume() * bc.GetPolarity());

  printf("Answer = %lld\n", ans);
  return 0;
}

