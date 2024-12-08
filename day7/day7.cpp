#include <iostream>
#include <print>
#include <vector>
#include <sstream>
#include <cmath>
#include <ranges>
#include <algorithm>
#include <functional>

using Num = uint64_t;

auto split(std::string line) {
  std::vector<Num> result{};
  Num n{};
  std::istringstream stream(std::move(line));
  while (stream >> n) {
    stream.ignore(1, ' ');
    result.push_back(n);
  }
  return result;
}

struct Equation {
  const Num test{};
  const std::vector<Num> numbers{};

  bool isValid(bool part1) const {
    return isValid(0, 0, part1);
  }
private:
  bool isValid(Num current, size_t index, bool part1) const {
    if (index == numbers.size()) {
      return current == test;
    }

    constexpr auto concat = [](const Num a, const Num b) -> Num {
      Num power = 10;
      for (; power <= b; power *= 10)
        ;
      return a * power + b;
    };

    const auto next = numbers[index];
    return isValid(current + next, index + 1, part1) || isValid(current * next, index + 1, part1) || (!part1 && isValid(concat(current, next), index + 1, part1));
  }
};

using Equations = std::vector<Equation>;

Equations parse() {
  std::string line{};
  Equations result{};
  while (std::getline(std::cin, line)) {
    auto numbers = split(line);
    const auto test = numbers[0];
    numbers.erase(numbers.begin());
    result.push_back({test, std::move(numbers)});
  }
  return result;
}

int main() {
  const auto equations = parse();
  std::println("Part 1 result = {}", std::ranges::fold_left(equations | std::views::filter([](const auto& e) { return e.isValid(true); }) | std::views::transform([](const auto& e) { return e.test; }), 0ull, std::plus()));
  std::println("Part 2 result = {}", std::ranges::fold_left(equations | std::views::filter([](const auto& e) { return e.isValid(false); }) | std::views::transform([](const auto& e) { return e.test; }), 0ull, std::plus()));
  return 0;
}
