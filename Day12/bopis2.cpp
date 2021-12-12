#include <map>
#include <vector>
#include <set>
#include <string>
#include <iostream>
#include <cctype>
#include <stack>

class graphNode
{
  public:
    graphNode(bool isBig) : isBig(isBig) {}

    void addNeighbor(int id) {
      neigh.insert(id);
    }

    const std::set<int>& getNeighbors() const {
      return neigh;
    }

    const int isSmall() const {
      return !isBig;
    }
  private:
    bool isBig;
    std::set<int> neigh;
};

typedef std::map<std::string, int> InputMap;
typedef std::vector<graphNode> Graph;

void AddNode(InputMap &map, Graph &graph, std::string &str)
{
  // First time seeing the string
  if (map.find(str) == map.end()) {
    map[str] = graph.size();
    bool isBig = std::isupper(str[0]);
    graph.push_back(graphNode(isBig));
  }
}

int visit(Graph &graph, std::vector<bool> &visited, int node, int start, int end, bool usedExtra)
{
  if (node == end)
    return 1;

  int ans = 0;
  visited[node] = true;
  for (int n: graph[node].getNeighbors()) {
    if (n == start)
      continue;

    if (graph[n].isSmall() && visited[n]) {
      if (usedExtra) {
        continue;
      } else {
        visited[n] = false;
        ans += visit(graph, visited, n, start, end, true);
        visited[n] = true;
      }
    } else {
      ans += visit(graph, visited, n, start, end, usedExtra);
    }
  }
  visited[node] = false;
  return ans;
}

int main(int argc, char *argv[])
{
  InputMap map;
  Graph graph;
  int start, end;

  while(!std::cin.eof()) {
    std::string buf;
    std::getline(std::cin, buf);
    
    if (buf.size() <= 0)
      break;

    std::string name[2] = {
      buf.substr(0, buf.find('-')),
      buf.substr(buf.find('-') + 1) };

    for (int i: {0, 1}) {
      AddNode(map, graph, name[i]);
      if (name[i].size() == 5)
        start = map[name[i]];
      if (name[i].size() == 3)
        end = map[name[i]];
    }

    int id[2] = { map[name[0]], map[name[1]] };
    graph[id[0]].addNeighbor(id[1]);
    graph[id[1]].addNeighbor(id[0]);
  }

  std::vector<bool> visited(graph.size(), false);
  int ans = visit(graph, visited, start, start, end, false);
  std::cout << "Answer: " << ans << std::endl;

  return 0;
}

