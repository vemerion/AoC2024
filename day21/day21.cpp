#include <iostream>
#include <string>
#include <print>
#include <vector>
#include <map>
#include <list>
#include <set>
#include <utility>

class Keypads {
  friend std::istream& operator>>(std::istream& is, Keypads& keypads);
public:
  void build() {
    static const std::vector<std::string> directionalKeypad{"     ",
                                                            "  ^A ",
                                                            " <v> ",
                                                            "     "};
    static const std::vector<std::string> numericalKeypad{"     ",
                                                          " 789 ",
                                                          " 456 ",
                                                          " 123 ",
                                                          "  0A ",
                                                          "     "};

    build(directionalKeypad);
    build(numericalKeypad);
  }
private:
  void build(const std::vector<std::string>& keypad) {
    static const std::map<std::pair<int, int>, char> directions{{{-1, 0}, '^'}, {{1, 0}, 'v'}, {{0, -1}, '<'}, {{0, 1}, '>'}};

    for (int row = 0; row < std::ssize(keypad); row++) {
      for (int col = 0; col < std::ssize(keypad[row]); col++) {
        const char start = keypad[row][col];
        if (start == ' ') {
          continue;
        }
        paths[start][start].push_back({'A'});
        struct Entry {
          int row{};
          int col{};
          std::string path{};
        };
        std::list list{Entry{row, col, ""}};
        std::set<char> visited{};
        while (!list.empty()) {
          const auto [r, c, path] = std::move(list.front());
          list.pop_front();
          visited.insert(keypad[r][c]);
          for (const auto& [dir, arrow] : directions) {
            const int nextRow = r + dir.first;
            const int nextCol = c + dir.second;
            const auto next = keypad[nextRow][nextCol];
            if (!visited.contains(next) && next != ' ') {
              list.push_back({nextRow, nextCol, path + arrow});
              paths[start][next].push_back({path + arrow + 'A'});
            }
          }
        }
      }
    }
  }

public:
  uint64_t complexities(uint64_t count) const {
    static constexpr std::array buttons{'A', '^', '>', 'v', '<'};

    std::map<char, std::map<char, uint64_t>> moves{};

    // User input
    for (const char start : buttons) {
      for (const char end : buttons) {
        moves[start][end] = paths.at(start).at(end).front().size();
      }
    }

    // Robots in the middle
    for (uint64_t i = 0; i < count - 1; i++) {
      std::map<char, std::map<char, uint64_t>> next{};
      for (const char start : buttons) {
        for (const char end : buttons) {
          uint64_t best{};
          for (const auto& path : paths.at(start).at(end)) {
            char prev = 'A';
            uint64_t steps{};
            for (const auto step : path) {
              steps += moves[prev][step];
              prev = step;
            }
            if (best == 0 || steps < best) {
              best = steps;
            }
          }
          next[start][end] = best;
        }
      }
      moves = std::move(next);
    }

    // Num pad
    uint64_t result{};
    for (const auto& code : codes) {
      uint64_t length{};
      char prev = 'A';
      for (const char c : code) {
        uint64_t best = 0;
        auto p = 'A';
        for (const auto& path : paths.at(prev).at(c)) {
          uint64_t steps{};
          for (const auto step : path) {
            steps += moves[p][step];
            p = step;
          }
          if (best == 0 || steps < best) {
            best = steps;
          }
        }
        length += best;
        prev = c;
      }
      result += length * std::stoull(code.substr(0, code.size() - 1));
    }

    return result;
  }
private:
  std::map<char, std::map<char, std::vector<std::string>>> paths{};
  std::vector<std::string> codes{};
};

std::istream& operator>>(std::istream& is, Keypads& keypads) {
  std::string line{};
  while (std::getline(is, line)) {
    keypads.codes.push_back(std::move(line));
  }
  return is;
}

int main() {
  Keypads keypads{};
  keypads.build();
  std::cin >> keypads;
  std::println("Part 1 result = {}", keypads.complexities(2));
  std::println("Part 2 result = {}", keypads.complexities(25));
  return 0;
}
