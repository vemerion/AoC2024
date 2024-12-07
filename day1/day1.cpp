#include <print>
#include <vector>
#include <iostream>
#include <algorithm>
#include <ranges>
#include <cmath>

auto parse() {
  std::vector<int> left{};
  std::vector<int> right{};
  int n1{};
  int n2{};
  while (std::cin >> n1 >> n2) {
    left.push_back(n1);
    right.push_back(n2);
  }
  std::ranges::sort(left);
  std::ranges::sort(right);
  return std::pair{left, right};
}

int part1(const auto& left, const auto& right) {
  return std::ranges::fold_left(std::ranges::views::zip_transform([](auto a, auto b) { return std::abs(a - b); }, left, right), 0, std::plus());
}

int part2(const auto& left, const auto& right) {
  return std::ranges::fold_left(left | std::ranges::views::transform([&right](auto a) { return a * std::ranges::count(right, a); }), 0, std::plus());
}

int main() {
  const auto [left, right] = parse();
  std::println("Part 1 result = {}", part1(left, right));
  std::println("Part 2 result = {}", part2(left, right));
  return 0;
}
