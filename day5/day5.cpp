#include <iostream>
#include <print>
#include <string>
#include <utility>
#include <vector>
#include <map>
#include <set>
#include <sstream>
#include <ranges>
#include <algorithm>
#include <functional>

using Orderings = std::map<int, std::set<int>>;
using Update = std::vector<int>;
using Updates = std::vector<Update>;

auto split(std::string line) {
  std::vector<int> result{};
  int n{};
  std::istringstream stream(std::move(line));
  while (stream >> n) {
    stream.ignore(1);
    result.push_back(n);
  }
  return result;
}

auto parse() {
  Orderings orderings{};
  std::string line{};
  while (std::getline(std::cin, line)) {
    if (line.empty()) {
      break;
    }
    const auto ordering = split(line);
    orderings[ordering[1]].insert(ordering[0]);
  }

  Updates updates{};
  while (std::getline(std::cin, line)) {
    const auto update = split(line);

    // Insert empty ordering if element has no ordering rules
    for (auto n : update) {
      orderings[n];
    }

    updates.push_back(std::move(update));
  }

  return std::pair{orderings, updates};
}

bool isValidUpdate(const Update& update, const Orderings& orderings) {
  std::set updated({update[0]});
  for (size_t i = 1; i < update.size(); i++) {
    const auto current = update[i];
    for (const auto prev : orderings.at(current)) {
      if ((std::ranges::find(update, prev) != update.end()) && !updated.contains(prev)) {
        return false;
      }
      updated.insert(current);
    }
  }

  return true;
}

Update reorder(const Update& update, const Orderings& orderings) {
  Update result{update.begin(), update.end()};
  for (const auto n : update) {
    result[std::ranges::count_if(update, [&orderings, n](auto e) { return orderings.at(n).contains(e); })] = n;
  }
  return result;
}

auto part1(const Orderings& orderings, const Updates& updates) {
  return std::ranges::fold_left(updates | std::views::filter([&orderings](const auto& u) { return isValidUpdate(u, orderings); }) | std::views::transform([](const auto& u) { return u[u.size() / 2]; }), 0, std::plus());
}

auto part2(const Orderings& orderings, const Updates& updates) {
  return std::ranges::fold_left(updates | std::views::filter([&orderings](const auto& u) { return !isValidUpdate(u, orderings); }) | std::views::transform([&orderings](const auto& u) { return reorder(u, orderings)[u.size() / 2]; }), 0, std::plus());
}

int main() {
  const auto [orderings, updates] = parse();
  std::println("Part 1 answer = {}", part1(orderings, updates));
  std::println("Part 2 answer = {}", part2(orderings, updates));
  return 0;
}
