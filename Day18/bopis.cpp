#include <iostream>
#include <iomanip>
#include <vector>
#include <cassert>
#include <algorithm>

const static int MAX_HEIGHT = 5;
const static int MAX_SIZE = 1 << (MAX_HEIGHT + 1);
const static int NOT_LEAF_VAL = -1;
const static int EXPLOSION_HEIGHT = 5; 
const static int INVALID_NODE = MAX_SIZE;

class SnailfishNumber {
  public:

    SnailfishNumber(std::string &str) : leafVal(MAX_SIZE, NOT_LEAF_VAL) {
      std::stringstream ss(str);
      // Binary tree root on index 1 for convenience
      ReadNext(ss, 1);
    }

    SnailfishNumber& operator+=(const SnailfishNumber &rhs) {
      // Not doing in-place for convenience
      SnailfishNumber res;
      res.CopyAsSubTree(*this, /*isLeft =*/ true);
      res.CopyAsSubTree(rhs, /*isLeft =*/ false);
      while (res.CheckExplosion() || res.CheckSplit()) {}
      *this = res;
      return *this;
    }

    // Not even necessary for debugging, but it's cute
    void DrawTree() const {
      int idx = 1;
      int nVals = 1;

      int treeHeight = this->GetTreeHeight();
      int treeSize = (1 << treeHeight);

      for (int i = 0; i < treeHeight; ++i) {
        int nSpacesMid = (treeSize * 2 - 1) / (1 << i);
        int nSpacesSide = (nSpacesMid - 1) / 2;

        std::cout << std::string(nSpacesSide, ' ');
        for (int j = 0; j < nVals; ++j) {
          if (leafVal[idx] == NOT_LEAF_VAL)
            if (this->HasLeafParent(idx))
              std::cout << ' ';
            else
              std::cout << 'o';
          else
            std::cout << leafVal[idx];
          idx += 1;
          if (j == nVals - 1)
            std::cout << std::string(nSpacesSide, ' ');
          else
            std::cout << std::string(nSpacesMid, ' ');
        }
        std::cout << '\n';
        nVals <<= 1;
      }
    }

    int GetTreeHeight(int node = 1, int subTreeHeight = 1) const {
      if (node > MAX_SIZE)
        return 0;
      else if (leafVal[node] == NOT_LEAF_VAL)
        return std::max(
            GetTreeHeight(node * 2, subTreeHeight + 1),
            GetTreeHeight(node * 2 + 1, subTreeHeight + 1));
      else
        return subTreeHeight;
    }

    const std::vector<int>& GetLeafVal() const {
      return leafVal;
    }

    const int GetMagnitude(int node = 1) const {
      if (leafVal[node] == NOT_LEAF_VAL)
        return GetMagnitude(node * 2) * 3 + GetMagnitude(node * 2 + 1) * 2;
      return leafVal[node];
    }

  private:
    std::vector<int> leafVal;

    // Does not support the public interface, should only be used internally
    SnailfishNumber() : leafVal(MAX_SIZE, NOT_LEAF_VAL) {}

    void ReadNext(std::stringstream &ss, int node) {
      char c;
      ss >> c;
      if (c == '[') {
        ReadNext(ss, node * 2);

        ss >> c;
        assert(c == ',');

        ReadNext(ss, node * 2 + 1);

        ss >> c;
        assert(c == ']');
      } else {
        int val = c - '0';
        leafVal[node] = val;
      }
    }

    void CopyAsSubTree(const SnailfishNumber &num, bool isLeft, int node = 1, int height = 1) {
      if(num.GetLeafVal()[node] == NOT_LEAF_VAL) {
        CopyAsSubTree(num, isLeft, node * 2, height + 1);
        CopyAsSubTree(num, isLeft, node * 2 + 1, height + 1);
      } else {
        int tgtNode = node | (1 << height);
        if (isLeft)
          tgtNode &= ~(1 << (height - 1));
        leafVal[tgtNode] = num.GetLeafVal()[node];
      }
    }

    bool HasLeafParent(int node) const {
      int parent = node / 2;
      if (parent < 1)
        return false;
      if (leafVal[parent] != NOT_LEAF_VAL)
        return true;
      return HasLeafParent(parent);
    }

    bool CheckExplosion() {
      int candidate = FindExplosionCandidate();
      if (candidate == INVALID_NODE)
        return false;

      Explode(candidate);
      return true;
    }

    // Note that this returns a non-leaf node unlike other functions here
    int FindExplosionCandidate(int node = 1, int height = 1) const {
      if (leafVal[node] == NOT_LEAF_VAL) {
        if (height == EXPLOSION_HEIGHT)
          return node;
        int candidate = FindExplosionCandidate(node * 2, height + 1);
        if (candidate != INVALID_NODE)
          return candidate;
        return FindExplosionCandidate(node * 2 + 1, height + 1);
      }
      return INVALID_NODE;
    }

    int FindLeftNeighbor(int node) const {
      // Find first parent that is a right son
      while (node % 2 == 0 && node > 1)
        node /= 2;
      if (node <= 1)
        return INVALID_NODE;

      // Find rightmost son of the sibling subtree
      node -= 1; 
      while (leafVal[node] == NOT_LEAF_VAL)
        node = (node * 2 + 1);
      return node;
    }

    int FindRightNeighbor(int node) const {
      // Find first parent that is a left son
      while (node % 2 == 1 && node > 1)
        node /= 2;
      if (node <= 1)
        return INVALID_NODE;

      // Find leftmost son of the sibling subtree
      node += 1; 
      while (leafVal[node] == NOT_LEAF_VAL)
        node = (node * 2);
      return node;
    }

    void Explode(int node) {
      int leftSon = node * 2;
      int rightSon = node * 2 + 1;
      
      int neigh = FindLeftNeighbor(leftSon);
      if (neigh != INVALID_NODE)
        leafVal[neigh] += leafVal[leftSon];
      neigh = FindRightNeighbor(rightSon);
      if (neigh != INVALID_NODE)
        leafVal[neigh] += leafVal[rightSon];
      leafVal[leftSon] = leafVal[rightSon] = NOT_LEAF_VAL;
      leafVal[node] = 0;
    }

    bool CheckSplit() {
      int candidate = FindSplitCandidate();
      if (candidate == INVALID_NODE)
        return false;
      Split(candidate);
      return true;
    }

    int FindSplitCandidate(int node = 1) const {
      if (leafVal[node] == NOT_LEAF_VAL) {
        int candidate = FindSplitCandidate(node * 2);
        if (candidate != INVALID_NODE)
          return candidate;
        else
          return FindSplitCandidate(node * 2 + 1);
      } else {
        if (leafVal[node] > 9)
          return node;
        else
          return INVALID_NODE;
      }
    }

    void Split(int node) {
      leafVal[node * 2] = leafVal[node] / 2;
      leafVal[node * 2 + 1] = (leafVal[node] / 2) + (leafVal[node] % 2);
      leafVal[node] = NOT_LEAF_VAL;
    }
};

int main(int argc, char *argv[])
{
  std::string buf;
  std::getline(std::cin, buf);
  SnailfishNumber num(buf);

  while (!std::cin.eof()) {
    std::getline(std::cin, buf);

    if (buf.size() < 2)
      break;

    SnailfishNumber rhs(buf);
    num += rhs;
  }

  std::cout << "Answer = " << num.GetMagnitude() << "\n";
  std::cout << "Enjoy the visual tree:\n";
  num.DrawTree();

  return 0;
}
