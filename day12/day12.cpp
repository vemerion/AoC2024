#include <iostream>
#include <print>
#include <string>
#include <set>
#include <list>
#include <array>
#include <vector>
#include <algorithm>
#include <functional>
#include <ranges>

struct Pos {
  int row{};
  int col{};

  auto operator<=>(const Pos&) const = default;

  Pos operator+(const Pos& other) const {
    return {row + other.row, col + other.col};
  }
  
  const auto perpendiculars() const {
    return row == 0 ? std::array{Pos{-1, 0}, Pos{1, 0}} : std::array{Pos{0, -1}, Pos{0, 1}};
  }
};

class Garden {
  friend std::istream& operator>>(std::istream& is, Garden& garden);
public:
  unsigned fences(bool part1) const {
    std::set<Pos> visited{};
    unsigned result{};
    for (int row = 0; row < std::ssize(grid); row++) {
      for (int col = 0; col < std::ssize(grid[row]); col++) {
        if (const auto pos = Pos{row, col}; !visited.contains(pos)) {
          const auto region = findRegion(pos);
          visited.insert(region.begin(), region.end());
          result += region.size() * (part1 ? perimeter(region) : sides(region));
        }
      }
    }
    return result;
  }
private:
  unsigned perimeter(const std::set<Pos>& region) const {
    return std::ranges::fold_left(region | std::views::transform([&region, this](const auto& p) {
      return std::ranges::fold_left(directions | std::views::transform([&p, type = at(p), this](const auto& d) { return at(p + d) != type; }), 0u, std::plus());
    }), 0u, std::plus());
  }

  unsigned sides(const std::set<Pos>& region) const {
    struct Fence {
      const Pos pos{};
      const Pos direction{};

      auto operator<=>(const Fence&) const = default;
    };
    std::set<Fence> visited{};
    unsigned result{};

    for (const auto& pos : region) {
      for (const auto& direction : directions) {
        if (at(pos) != at(pos + direction) && !visited.contains({pos, direction})) {
          result++;
          for (const auto& perpendicular : direction.perpendiculars()) {
            for (auto current = pos; at(current) == at(pos) && at(current) != at(current + direction); current = current + perpendicular) {
              visited.insert({current, direction});
            }
          }
        }
      }
    }
    return result;
  }

  std::set<Pos> findRegion(const Pos& start) const {
    std::set<Pos> result{start};
    std::list<Pos> list{start};
    const char type = at(start);

    while (!list.empty()) {
      const auto current = list.front();
      list.pop_front();
      for (const auto& direction : directions) {
        if (const auto next = current + direction; at(next) == type && !result.contains(next)) {
          list.push_back(next);
          result.insert(next);
        }
      }
    }
    return result;
  }

  bool isInside(const Pos& p) const {
    return p.row >= 0 && p.row < std::ssize(grid) && p.col >= 0 && p.col < std::ssize(grid[p.row]);
  }

  char at(const Pos& p) const {
    return isInside(p) ? grid[p.row][p.col] : 0;
  }

  static constexpr std::array directions{Pos{-1, 0}, Pos{1, 0}, Pos{0, -1}, Pos{0, 1}};
  std::vector<std::string> grid{};
};

std::istream& operator>>(std::istream& is, Garden& garden) {
  std::string line{};
  while (std::getline(is, line)) {
    garden.grid.push_back(std::move(line));
  }
  return is;
}

int main() {
  Garden garden{};
  std::cin >> garden;
  std::println("Part 1 result = {}", garden.fences(true));
  std::println("Part 2 result = {}", garden.fences(false));
  return 0;
}
