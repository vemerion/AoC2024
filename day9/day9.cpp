#include <iostream>
#include <string>
#include <print>
#include <list>
#include <algorithm>
#include <functional>
#include <ranges>

struct Block {
  int id = -1;
  uint64_t min{};
  uint64_t max{};

  uint64_t count() const {
    return max < min ? 0 : max - min + 1;
  }

  bool isFree() const {
    return id == -1;
  }

  uint64_t value() const {
    return isFree() ? 0 : std::ranges::fold_left(std::views::iota(min, max + 1) | std::views::transform([this](auto e) { return e * id; }), 0ull, std::plus());
  }
};

using Blocks = std::list<Block>;

Blocks parse() {
  Blocks result{};
  std::string line{};
  std::getline(std::cin, line);
  int id{};
  uint64_t pos{};
  for (size_t i = 0; i < line.size(); i++) {
    uint64_t count = line[i] - '0';
    result.push_back({i % 2 == 0 ? id++ : -1, pos, (pos += count) - 1});
  }
  return result;
}

uint64_t compact(Blocks blocks, bool part1) {
  auto left = blocks.begin();
  auto right = blocks.rbegin();
  while ((!part1 && right != blocks.rend()) || (part1 && left != std::prev(right.base()))) {
    if (left == std::prev(right.base())) {
      left = blocks.begin();
      right++;
      continue;
    }
    if (right->isFree() || right->count() == 0) {
      right++;
      continue;
    }
    if (!left->isFree()) {
      left++;
      continue;
    }
    if (!part1 && left->count() < right->count()) {
      left++;
      continue;
    }

    const auto count = std::min(left->count(), right->count());

    // Left will not be completely filled so needs to be split
    if (left->count() != count) {
      blocks.insert(std::next(left), {-1, left->min + count, left->max});
    }

    left->id = right->id;
    left->max = left->min + count - 1;
    right->max -= count;
    if (!part1) {
      left = blocks.begin();
      right++;
    }
  }

  return std::ranges::fold_left(blocks | std::views::transform([](const auto& b) { return b.value(); }), 0ull, std::plus());
}

int main() {
  auto blocks = parse();
  std::println("Part 1 result = {}", compact(blocks, true));
  std::println("Part 2 result = {}", compact(blocks, false));
  return 0;
}
