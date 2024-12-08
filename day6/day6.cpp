#include <iostream>
#include <print>
#include <string>
#include <vector>
#include <map>
#include <set>

class Grid{
friend std::istream& operator>>(std::istream& is, Grid& grid);
public:
  int patrol(bool part1) const {
    static constexpr std::array<Pos, 4> directions{Pos{-1, 0}, Pos{0, 1}, Pos{1, 0}, Pos{0, -1}};
    std::map<Pos, std::set<int>> visited{};
    size_t direction{};
    auto pos = startPosition;

    while (isInside(pos)) {
      const auto next = pos + directions[direction];
      if (isObstacle(next)) {
        direction = (direction + 1) % directions.size();

        // Stuck in loop?
        if (const auto& v = visited[pos]; v.contains(direction)) {
          return -1;
        }

        if (!part1) {
          visited[pos].insert(direction);
        }
      } else {
        if (part1) {
          visited[pos].insert(direction);
        }

        pos = next;
      }
    }

    return visited.size();
  }

  int part2() {
    int count{};
    for (int row = 0; row < std::ssize(grid); row++) {
      for (int col = 0; col < std::ssize(grid[row]); col++) {
        if (const auto obstacle = Pos{row, col}; !isObstacle(obstacle) && obstacle != startPosition) {
          extraObstacle = obstacle;
          count += patrol(false) == -1;
        }
      }
    }
    return count;
  }
private:
  struct Pos {
    int row{};
    int col{};

    Pos operator+(const Pos& other) const {
      return {row + other.row, col + other.col};
    }

    auto operator<=>(const Pos&) const = default;
  };
  
  bool isInside(const Pos& p) const {
    return p.row >= 0 && p.row < std::ssize(grid) && p.col >= 0 && p.col < std::ssize(grid[p.row]);
  }

  bool isObstacle(const Pos& p) const {
    return p == extraObstacle || (isInside(p) ? grid[p.row][p.col] == '#' : false);
  }

  std::vector<std::string> grid{};
  Pos startPosition{};
  Pos extraObstacle{-1, -1};
};

std::istream& operator>>(std::istream& is, Grid& grid) {
  std::string line{};
  while (std::getline(is, line)) {
    if (const auto col = line.find('^'); col != std::string::npos) {
      grid.startPosition = {static_cast<int>(grid.grid.size()), static_cast<int>(col)};
    }
    grid.grid.push_back(std::move(line));
  }
  return is;
}

int main() {
  Grid grid{};
  std::cin >> grid;
  std::println("Part 1 result = {}", grid.patrol(true));
  std::println("Part 2 result = {}", grid.part2());
  return 0;
}
