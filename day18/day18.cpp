#include <iostream>
#include <string>
#include <print>
#include <format>
#include <vector>
#include <list>
#include <set>
#include <array>
#include <optional>
#include <cassert>

struct Pos {
  int x{};
  int y{};

  auto operator<=>(const Pos&) const = default;

  Pos operator+(const Pos& other) const {
    return {x + other.x, y + other.y};
  }
};

constexpr std::array directions{Pos{-1, 0}, Pos{1, 0}, Pos{0, -1}, Pos{0, 1}};

class Memory {
  friend std::istream& operator>>(std::istream& is, Memory& memory);
public:
  std::string part2() const {
    size_t min = 0;
    size_t max = bytes.size();
    while (true) {
      const auto middle = (min + max) / 2;
      if (path(middle)) {
        min = middle + 1;
      } else {
        max = middle - 1;
      }
      if (min > max) {
        const auto result = bytes[!path(middle) ? middle - 1 : middle];
        return std::format("{},{}", result.x, result.y);
      }
    }
    assert(false);
  }
  std::optional<unsigned> path(size_t falling) const {
    struct Move {
      const Pos pos{};
      const unsigned step{};
    };
    std::set<Pos> obstacles{};
    std::set<Pos> visited{start};
    std::list list{Move{start, 0}};

    // Fill obstacles
    for (size_t i = 0; i < falling; i++) {
      obstacles.insert(bytes[i]);
    }

    while (!list.empty()) {
      const auto [pos, step] = list.front();
      list.pop_front();

      if (pos == end) {
        return step;
      }

      for (const auto& direction : directions) {
        if (const auto next = pos + direction; isInside(next) && !visited.contains(next) && !obstacles.contains(next)) {
          visited.insert(next);
          list.push_back({next, step + 1});
        }
      }
    }
    return {};
  }
private:
  bool isInside(const Pos& p) const {
    return p.x >= 0 && p.x <= end.x && p.y >= 0 && p.y <= end.y;
  }

  static constexpr Pos start{0, 0};
  static constexpr Pos end{70, 70};
  std::vector<Pos> bytes{};
};

std::istream& operator>>(std::istream& is, Memory& memory) {
  Pos pos{};
  char unused{};
  while (is >> pos.x >> unused >> pos.y) {
    memory.bytes.push_back(pos);
  }
  return is;
}

int main() {
  Memory memory{};
  std::cin >> memory;
  std::println("Part 1 result = {}", *memory.path(1024));
  std::println("Part 2 result = {}", memory.part2());
  return 0;
}
