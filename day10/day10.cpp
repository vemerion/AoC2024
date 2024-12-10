#include <iostream>
#include <print>
#include <string>
#include <list>
#include <vector>
#include <set>

struct Pos {
  int row{};
  int col{};

  auto operator<=>(const Pos&) const = default;

  Pos operator+(const Pos& other) const {
    return {row + other.row, col + other.col};
  }
};

class Grid {
  friend std::istream& operator>>(std::istream& is, Grid& grid);
public:
  unsigned paths(bool part1) const {
    unsigned count{};

    for (int row = 0; row < std::ssize(grid); row++) {
      for (int col = 0; col < std::ssize(grid[row]); col++) {
        if (const auto pos = Pos{row, col}; at(pos) == '0') {
          std::list list{pos};
          std::set<Pos> destinations{};
          while (!list.empty()) {
            const auto current = list.front();
            list.pop_front();
            if (at(current) == '9') {
              destinations.insert(current);
              if (!part1) {
                count++;
              }
              continue;
            }
            for (const auto& direction : directions) {
              if (const auto next = current + direction; at(next) == at(current) + 1) {
                list.push_back(next);
              }
            }
          }
          if (part1) {
            count += destinations.size();
          }
        }
      }
    }

    return count;
  }

private:
  bool isInside(const Pos& p) const {
    return p.row >= 0 && p.row < std::ssize(grid) && p.col >= 0 && p.col < std::ssize(grid[p.row]);
  }

  char at(const Pos& p) const {
    return !isInside(p) ? 0 : grid[p.row][p.col];
  }

  static constexpr std::array directions{Pos{-1, 0}, Pos{1, 0}, Pos{0, -1}, Pos{0, 1}};

  std::vector<std::string> grid{};
};

std::istream& operator>>(std::istream& is, Grid& grid) {
  std::string line{};
  while (std::getline(is, line)) {
    grid.grid.push_back(std::move(line));
  }
  return is;
}

int main() {
  Grid grid{};
  std::cin >> grid;
  std::println("Part 1 result = {}", grid.paths(true));
  std::println("Part 2 result = {}", grid.paths(false));
  return 0;
}
