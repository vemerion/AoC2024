#include <iostream>
#include <string>
#include <print>
#include <vector>
#include <map>
#include <list>
#include <ranges>
#include <functional>
#include <algorithm>

using Num = int64_t;
using Secrets = std::vector<Num>;

Secrets parse() {
  Secrets secrets{};
  Num n{};
  while (std::cin >> n) {
    secrets.push_back(n);
  }
  return secrets;
}

Secrets generate(Num n, Num steps) {
  static constexpr Num prune = 16777216;
  Secrets generated{};
  for (Num i = 0; i < steps; i++) {
    n = ((n * 64) ^ n) % prune;
    n = ((n / 32) ^ n) % prune;
    n = ((n * 2048) ^ n) % prune;
    generated.push_back(n);
  }
  return generated;
}

Num part1(const Secrets& secrets) {
  return std::ranges::fold_left(secrets | std::views::transform([](const auto s) { return generate(s, 2000).back(); }), 0, std::plus());
}

std::map<std::list<Num>, Num> findSequences(const Num secret) {
  std::map<std::list<Num>, Num> sequences{};
  std::list<Num> sequence{};
  Num prev = secret % 10;
  for (const auto n : generate(secret, 2000)) {
    const auto bananas = n % 10;
    sequence.push_back(bananas - prev);
    if (sequence.size() > 4) {
      sequence.pop_front();
    }
    if (sequence.size() == 4 && !sequences.contains(sequence)) {
      sequences[sequence] = bananas;
    }
    prev = bananas;
  }
  return sequences;
}

Num part2(const Secrets& secrets) {
  std::map<std::list<Num>, Num> sequences{};
  for (const auto& secret : secrets) {
    for (const auto& [seq, price] : findSequences(secret)) {
      sequences[seq] += price;
    }
  }

  return *std::ranges::max_element(sequences | std::views::values);
}

int main() {
  const auto secrets = parse();
  std::println("Part 1 result = {}", part1(secrets));
  std::println("Part 2 result = {}", part2(secrets));
  return 0;
}
