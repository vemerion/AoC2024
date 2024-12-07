#include <print>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <ranges>
#include <algorithm>

using Report = std::vector<int>;
using Reports = std::vector<Report>;

auto split(std::string line) {
  std::vector<int> result{};
  int n{};
  std::istringstream stream(std::move(line));
  while (stream >> n) {
    result.push_back(n);
  }
  return result;
}

Reports parse() {
  Reports reports{};
  std::string line{};
  while (std::getline(std::cin, line)) {
    reports.push_back(split(std::move(line)));
  }
  return reports;
}

bool isSafe(const Report& report) {
  const auto range = report | std::ranges::views::pairwise | std::ranges::views::transform([](auto t) { return std::get<0>(t) - std::get<1>(t); });
  return std::ranges::all_of(range, [](auto e) { return e < 0 && e > -4; }) || std::ranges::all_of(range, [](auto e) { return e > 0 && e < 4; });
}

bool isSafeDampener(const Report& report) {
  for (size_t i = 0; i < report.size(); i++) {
    Report dampened{};
    dampened.insert(std::end(dampened), std::begin(report), std::begin(report) + i);
    dampened.insert(std::end(dampened), std::begin(report) + i + 1, std::end(report));
    if (isSafe(dampened)) {
      return true;
    }
  }
  return false;
}

int main() {
  const auto reports = parse();
  std::println("Part 1 result = {}", std::ranges::count_if(reports, isSafe));
  std::println("Part 2 result = {}", std::ranges::count_if(reports, isSafeDampener));
  return 0;
}
