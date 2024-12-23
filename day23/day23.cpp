#include <iostream>
#include <string>
#include <print>
#include <set>
#include <map>
#include <ranges>
#include <algorithm>
#include <functional>

struct Computer {
  std::set<std::string> edges{};
};

using Computers = std::map<std::string, Computer>;
using Parties = std::set<std::set<std::string>>;

Computers parse() {
  Computers computers{};
  std::string line{};
  while (std::getline(std::cin, line)) {
    std::string c1{line[0], line[1]};
    std::string c2{line[3], line[4]};
    computers[c1].edges.insert(c2);
    computers[c2].edges.insert(c1);
  }
  return computers;
}

unsigned part1(const Parties& parties) {
  return std::ranges::count_if(parties, [](const auto& p) { return p.size() == 3 && std::ranges::any_of(p, [](const auto& e) { return e[0] == 't'; }); });
}

void findParties(const Computers& computers, Parties& parties, const std::set<std::string>& party) {
  if (parties.contains(party)) {
    return;
  }

  if (party.size() > 2) {
    parties.insert(party);
  }

  for (const auto& [name, computer] : computers) {
    if (party.contains(name)) {
      continue;
    }

    if (std::ranges::all_of(party, [&computers, &name](const auto& e) { return computers.at(e).edges.contains(name); })) {
      std::set next(party.begin(), party.end());
      next.insert(name);
      findParties(computers, parties, next);
    }
  }
}

Parties findParties(const Computers& computers) {
  Parties result{};

  for (const auto& [name, computer] : computers) {
    std::set<std::string> party{name};
    findParties(computers, result, party);
  }

  return result;
}

std::string part2(const Parties& parties) {
  return std::ranges::fold_left(*std::ranges::max_element(parties, [](const auto& a, const auto& b) { return a.size() < b.size(); }), std::string{}, [](const auto& a, const auto& b) { return a + ',' + b; }).substr(1);
}

int main() {
  const auto computers = parse();
  const auto parties = findParties(computers);
  std::println("Part 1 result = {}", part1(parties));
  std::println("Part 2 result = {}", part2(parties));
  return 0;
}
