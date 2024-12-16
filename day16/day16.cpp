#include <iostream>
#include <vector>
#include <string>
#include <array>
#include <queue>
#include <map>
#include <set>
#include <list>
#include <print>
#include <limits>
#include <cassert>

struct Pos {
  int row{};
  int col{};

  auto operator<=>(const Pos&) const = default;

  Pos operator+(const Pos& other) const {
    return {row + other.row, col + other.col};
  }
};

constexpr std::array directions{Pos{0, 1}, Pos{1, 0}, Pos{0, -1}, Pos{-1, 0}};

struct Node;

struct Edge {
  Node* node{};
  unsigned cost{};
};

struct Node {
  unsigned cost{std::numeric_limits<unsigned>::max()};
  bool visited{};
  std::vector<Node*> prevs{};

  bool end{};
  std::vector<Edge> edges{};
  Pos pos{};
};

class Maze {
friend std::istream& operator>>(std::istream& is, Maze& maze);
public:
  std::pair<unsigned, unsigned> path() {
    constexpr auto cmp = [](const Node* n1, const Node* n2) { return n1->cost > n2->cost; };
    std::priority_queue<Node*, std::vector<Node*>, decltype(cmp)> unvisited{};
    start->cost = 0;
    unvisited.push(start);
    unsigned best{};
    std::set<Pos> paths{};
    std::list<const Node*> list{};

    while (true) {
      auto node = unvisited.top();
      unvisited.pop();
      if (node->visited) {
        continue;
      }
      for (const auto& edge : node->edges) {
        if (edge.node->visited) {
          continue;
        }
        if (node->cost + edge.cost <= edge.node->cost) {
          edge.node->cost = node->cost + edge.cost;
          edge.node->prevs.push_back(node);
          unvisited.push(edge.node);
        }
      }
      node->visited = true;

      if (!node->end && best != 0) {
        while (!list.empty()) {
          const auto n = list.front();
          list.pop_front();
          paths.insert(n->pos);
          for (const auto& prev : n->prevs) {
            list.push_back(prev);
          }
        }
        return {best, paths.size()};
      }

      if (node->end) {
        best = node->cost;
        list.push_back(node);
      }
    }
    assert(false);
  }
private:
  std::map<Pos, std::array<Node, directions.size()>> nodes{};
  Node* start{};
};

std::istream& operator>>(std::istream& is, Maze& maze) {
  std::string line{};
  std::vector<std::string> grid{};
  while (std::getline(is, line)) {
    grid.push_back(std::move(line));
  }

  for (int row = 0; row < std::ssize(grid); row++) {
    for (int col = 0; col < std::ssize(grid[row]); col++) {
      const char c = grid[row][col];
      if (c == '#') {
        continue;
      }
      maze.nodes[{row, col}];
    }
  }

  for (auto& [pos, nodes] : maze.nodes) {
    if (grid[pos.row][pos.col] == 'S') {
      maze.start = &nodes[0];
    }
    for (size_t i = 0; i < nodes.size(); i++) {
      auto& node = nodes[i];
      node.pos = pos;
      if (grid[pos.row][pos.col] == 'E') {
        node.end = true;
      }

      node.edges.push_back({&nodes[(i + 1) % directions.size()], 1000});
      node.edges.push_back({&nodes[(i + 3) % directions.size()], 1000});
      if (const auto adjacent = pos + directions[i]; grid[adjacent.row][adjacent.col] != '#') {
        node.edges.push_back({&maze.nodes[adjacent][i], 1});
      }
    }
  }

  return is;
}

int main() {
  Maze maze{};
  std::cin >> maze;
  const auto [part1, part2] = maze.path();
  std::println("Part 1 result = {}", part1);
  std::println("Part 2 result = {}", part2);
}
