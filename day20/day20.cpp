#include <iostream>
#include <string>
#include <vector>
#include <print>
#include <array>
#include <set>
#include <list>

struct Pos {
  int row{};
  int col{};

  auto operator<=>(const Pos&) const = default;

  Pos operator+(const Pos& other) const {
    return {row + other.row, col + other.col};
  }
};

constexpr std::array directions{Pos{-1, 0}, Pos{1, 0}, Pos{0, -1}, Pos{0, 1}};

struct Tile {
  bool wall{};
  int distance = -1;
};

class Grid {
  friend std::istream& operator>>(std::istream& is, Grid& grid);
public:
  unsigned cheats(int max) {
    unsigned result{};
    for (int row = 0; row < std::ssize(grid); row++) {
      for (int col = 0; col < std::ssize(grid[row]); col++) {
        struct Entry {
          const Pos pos{};
          const int steps{};
        };
        const auto pos = Pos{row, col};
        std::set<Pos> visited{};
        std::list list{Entry{pos, 0}};
        while (!list.empty()) {
          const auto [current, steps] = list.front();
          list.pop_front();
          for (const auto& direction : directions) {
            if (const auto next = current + direction; steps < max && isInside(next) && !visited.contains(next)) {
              visited.insert(next);
              list.push_back({next, steps + 1});
              if (const auto& tile = at(next); !tile.wall && at(pos).distance - tile.distance - steps >= 100) {
                result++;
              }
            }
          }
        }
      }
    }
    return result;
  }
private:
  bool isInside(const Pos& pos) const {
    return pos.row >= 0 && pos.row < std::ssize(grid) && pos.col >= 0 && pos.col < std::ssize(grid[pos.row]);
  }

  Tile& at(const Pos& pos) {
    return grid[pos.row][pos.col];
  }
  
  std::vector<std::vector<Tile>> grid{};
  Pos start{};
  Pos end{};
};

std::istream& operator>>(std::istream& is, Grid& grid) {
  std::string line{};
  while (std::getline(is, line)) {
    grid.grid.push_back({});
    for (int col = 0; col < std::ssize(line); col++) {
      const char c = line[col];
      grid.grid.back().push_back({.wall = c == '#', .distance = -1});
      if (c == 'S') {
        grid.start = {static_cast<int>(grid.grid.size()) - 1, col};
      } else if (c == 'E') {
        grid.end = {static_cast<int>(grid.grid.size()) - 1, col};
      }
    }
  }

  auto pos = grid.end;
  grid.at(pos).distance = 0;
  while (true) {
    for (const auto& direction : directions) {
      const auto next = pos + direction;
      if (auto& tile = grid.at(next); !tile.wall && tile.distance == -1) {
        tile.distance = grid.at(pos).distance + 1;
        pos = next;
      }
    }
    if (pos == grid.start) {
      break;
    }
  }
  return is;
}

int main() {
  Grid grid;
  std::cin >> grid;
  std::println("Part 1 result = {}", grid.cheats(2));
  std::println("Part 2 result = {}", grid.cheats(20));
  return 0;
}
