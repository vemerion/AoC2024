#include <iostream>
#include <string>
#include <print>
#include <vector>
#include <regex>
#include <cmath>
#include <functional>
#include <ranges>
#include <algorithm>

using Num = int64_t;

struct Pos {
  Num x{};
  Num y{};
};

struct Machine {
  Pos a{};
  Pos b{};
  Pos prize{};

  Pos& operator[](size_t i) {
    return i == 0 ? a : (i == 1 ? b : prize);
  }

  Num play(bool part1) const {
    const auto target = part1 ? prize : Pos{prize.x + offset, prize.y + offset};
    const Num n1 = std::round((target.x - target.y * b.x / static_cast<double>(b.y)) / (a.x - a.y * b.x / static_cast<double>(b.y)));
    const Num n2 = std::round((target.y - n1 * static_cast<double>(a.y)) / b.y);
    if (n1 * a.x + n2 * b.x == target.x && n1 * a.y + n2 * b.y == target.y) {
      return n1 * 3 + n2 * 1;
    } else {
      return 0;
    }
  }
private:
  static constexpr Num offset = 10000000000000ll;
};

using Machines = std::vector<Machine>;

std::vector<Num> findNumbers(std::string& s) {
  std::vector<Num> result{};
  std::regex regex{"(\\d+)"};
  auto numbers = std::sregex_iterator(s.begin(), s.end(), regex);
  for (auto itr = numbers; itr != std::sregex_iterator(); itr++) {
    result.push_back(std::stoi(itr->str()));
  }
  return result;
}

Num play(const Machines& machines, bool part1) {
  return std::ranges::fold_left(machines | std::views::transform([part1](const auto& m) { return m.play(part1); }), 0ll, std::plus());
}

Machines parse() {
  Machines result{};
  std::string line{};
  while (!std::cin.eof()) {
    Machine machine{};
    for (size_t i = 0; i < 3; i++) {
      std::getline(std::cin, line);
      const auto numbers = findNumbers(line);
      machine[i] = {numbers[0], numbers[1]};
    }
    result.push_back(machine);
    std::getline(std::cin, line);
  }
  return result;
}

int main() {
  const auto machines = parse();
  std::println("Part 1 result = {}", play(machines, true));
  std::println("Part 2 result = {}", play(machines, false));
  return 0;
}
