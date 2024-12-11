#include <iostream>
#include <string>
#include <print>
#include <unordered_map>
#include <algorithm>
#include <ranges>
#include <functional>

using Rock = uint64_t;
using Num = uint64_t;
using Rocks = std::unordered_map<Rock, Num>;

Rocks parse() {
  Rock n{};
  Rocks rocks{};
  while (std::cin >> n) {
    rocks[n]++;
  }
  return rocks;
}

Num blink(Rocks rocks, Num blinks) {
  for (Num i = 0; i < blinks; i++) {
    Rocks next{};
    for (const auto& [rock, count] : rocks) {
      if (rock == 0) {
        next[1] += count;
      } else if (const auto s = std::to_string(rock); s.size() % 2 == 0) {
        const auto half = s.size() / 2;
        next[std::stoull(s.substr(0, half))] += count;
        next[std::stoull(s.substr(half, half))] += count;
      } else {
        next[rock * 2024] += count;
      }
    }
    rocks = std::move(next);
  }
  return std::ranges::fold_left(rocks | std::views::values, 0ull, std::plus());
}

int main() {
  const auto rocks = parse();
  std::println("Part 1 result = {}", blink(rocks, 25));
  std::println("Part 2 result = {}", blink(rocks, 75));
  return 0;
}
