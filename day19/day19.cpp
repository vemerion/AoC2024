#include <iostream>
#include <string>
#include <print>
#include <vector>

class Onsen {
  friend std::istream& operator>>(std::istream& is, Onsen& onsen);
public:
  size_t possibleDesigns(bool part1) const {
    size_t result{};
    for (const auto& design : designs) {
      const auto count = arrangements(design);
      result += part1 ? count != 0 : count;
    }
    return result;
  }

private:
  size_t arrangements(const std::string& design) const {
    std::vector<size_t> counts(design.size() + 1, 0);
    counts[0] = 1;
    for (size_t i = 0; i < design.size(); i++) {
      const auto count = counts[i];
      if (count == 0) {
        continue;
      }

      for (const auto& towel : towels) {
        if ((design.size() - i >= towel.size()) && (std::string_view{design.data() + i, towel.size()} == towel)) {
          counts[i + towel.size()] += count;
        }
      }
    }
    
    return counts[design.size()];
  }

  std::vector<std::string> towels{};
  std::vector<std::string> designs{};
};

std::istream& operator>>(std::istream& is, Onsen& onsen) {
  std::string word{};
  bool towels = true;
  while (is >> word) {
    if (towels && word.back() != ',') {
      towels = false;
      onsen.towels.push_back(std::move(word));
      continue;
    }
    if (towels) {
      onsen.towels.push_back(word.substr(0, word.size() - 1));
    } else {
      onsen.designs.push_back(std::move(word));
    }
  }
  return is;
}

int main() {
  Onsen onsen{};
  std::cin >> onsen;
  std::println("Part 1 result = {}", onsen.possibleDesigns(true));
  std::println("Part 2 result = {}", onsen.possibleDesigns(false));
  return 0;
}
