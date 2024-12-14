#include <iostream>
#include <string>
#include <print>
#include <vector>
#include <set>
#include <map>
#include <regex>
#include <functional>
#include <algorithm>
#include <ranges>
#include <optional>

std::vector<int> findNumbers(std::string& s) {
  std::vector<int> result{};
  std::regex regex{"(-?\\d+)"};
  auto numbers = std::sregex_iterator(s.begin(), s.end(), regex);
  for (auto itr = numbers; itr != std::sregex_iterator(); itr++) {
    result.push_back(std::stoi(itr->str()));
  }
  return result;
}

static constexpr int width = 101;
static constexpr int height = 103;

struct Pos {
  int x{};
  int y{};

  auto operator<=>(const Pos&) const = default;
};

struct Robot {
  Pos pos{};
  Pos velocity{};

  void move(unsigned steps) {
    int x = pos.x + velocity.x * steps;
    int y = pos.y + velocity.y * steps;
    pos.x = (width + (x % width)) % width;
    pos.y = (height + (y % height)) % height;
  }

  std::optional<Pos> quadrant() const {
    if (pos.x == width / 2 || pos.y == height / 2) {
      return {};
    }

    return Pos{pos.x / (width / 2 + 1), pos.y / (height / 2 + 1)};
  }
};

using Robots = std::vector<Robot>;

Robots parse() {
  Robots result{};
  std::string line{};
  while (std::getline(std::cin, line)) {
    const auto numbers = findNumbers(line);
    result.push_back({.pos = {numbers[0], numbers[1]}, .velocity = {numbers[2], numbers[3]}});
  }
  return result;
}

unsigned part1(Robots robots) {
  for (auto& robot : robots) {
    robot.move(100);
  }

  std::map<Pos, unsigned> quadrants{};

  for (const auto& robot : robots) {
    if (const auto quadrant = robot.quadrant(); quadrant) {
      quadrants[*quadrant]++; 
    }
  }
  return std::ranges::fold_left(quadrants | std::views::values, 1u, std::multiplies());
}

unsigned part2(Robots robots) {
  std::vector<std::string> rows{};
  std::set<Pos> set{};
  for (unsigned row = 0; row < height; row++) {
    rows.push_back(std::string(width, ' '));
  }
  for (unsigned i = 1;; i++) {
    for (auto& robot : robots) {
      rows[robot.pos.y][robot.pos.x] = ' ';
      robot.move(1);
      rows[robot.pos.y][robot.pos.x] = '#';
      set.insert(robot.pos);
    }

    unsigned count{};
    for (const auto& a : set) {
      /* Idea: tree should be shaped something like this
              /\
             /  \
            /    \
           /      \
      */
      if (set.contains({a.x + 1, a.y - 1}) || set.contains({a.x + 1, a.y + 1})) {
        count++;
      }
    }
    if (count > set.size() * 0.35) {
      for (const auto& row : rows) {
        std::println("{}", row);
      }
      return i;
    }
    set.clear();
  }
}

int main() {
  const auto robots = parse();
  std::println("Part 1 result = {}", part1(robots));
  std::println("Part 2 result = {}", part2(robots));
  return 0;
}
