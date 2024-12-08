#include <iostream>
#include <print>
#include <string>
#include <map>
#include <vector>
#include <set>
#include <array>
#include <utility>

struct Pos {
  int row{};
  int col{};

  auto operator<=>(const Pos&) const = default;

  Pos operator+(const Pos& other) const {
    return {row + other.row, col + other.col};
  }

  Pos operator-(const Pos& other) const {
    return {row - other.row, col - other.col};
  }
};

using Line = std::pair<Pos, Pos>;

class Grid {
friend std::istream& operator>>(std::istream& is, Grid& grid);
public:
  int antinodes(bool part1) const {
    std::set<Pos> positions{};
    for (const auto& [c, v] : antennas) {
      for (size_t i = 0; i < v.size(); i++) {
        for (size_t j = i + 1; j < v.size(); j++) {
          const auto& a1 = v[i];
          const auto& a2 = v[j];
          std::array<Line, 2> lines{Line{a1, a1 - a2}, Line{a2, a2 - a1}};
          if (part1) {
            for (const auto& [pos, dir] : lines) {
              if (const auto p = pos + dir; isInside(p)) {
                positions.insert(p);
              }
            }
          } else {
            for (const auto& [pos, dir] : lines) {
              auto p = pos;
              while (isInside(p)) {
                positions.insert(p);
                p = p + dir;
              }
            }
          }
        }
      }
    }
    return positions.size();
  }

private:
  bool isInside(const Pos& p) const {
    return p.row >= 0 && p.row < limits.row && p.col >= 0 && p.col < limits.col;
  }

  std::map<char, std::vector<Pos>> antennas{};
  Pos limits{};
};

std::istream& operator>>(std::istream& is, Grid& grid) {
  std::string line{};
  int row{};
  int colLimit{};
  while (std::getline(is, line)) {
    for (int col = 0; col < std::ssize(line); col++) {
      const auto c = line[col];
      if (c != '.') {
        grid.antennas[c].push_back({row, col});
      }
    }
    colLimit = static_cast<int>(line.size());
    row++;
  }
  grid.limits = {row, colLimit};
  return is;
}

int main() {
  Grid grid{};
  std::cin >> grid;
  std::println("Part 1 result = {}", grid.antinodes(true));
  std::println("Part 2 result = {}", grid.antinodes(false));
  return 0;
}
