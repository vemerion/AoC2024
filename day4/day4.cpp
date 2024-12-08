#include <iostream>
#include <print>
#include <format>
#include <string>
#include <vector>
#include <array>

class Grid {
friend std::istream& operator>>(std::istream& is, Grid& grid);

public:
  int xmas(bool part1) const {
    int count{};
    for (int row = 0; row < std::ssize(grid); row++) {
      for (int col = 0; col < std::ssize(grid[row]); col++) {
        count += part1 ? countPart1(row, col) : countPart2(row, col);
      }
    }
    return count;
  }

private:
  int countPart1(int row, int col) const {
    int count{};
    for (int rowOffset = -1; rowOffset <= 1; rowOffset++) {
      for (int colOffset = -1; colOffset <= 1; colOffset++) {
        std::string text{};
        for (int i = 0; i < 4; i++) {
          text += at(row + rowOffset * i, col + colOffset * i);
        }
        if (text == "XMAS") {
          count++;
        }
      }
    }
    return count;
  }

  int countPart2(int row, int col) const {
    std::string word1 = std::format("{}{}{}", at(row - 1, col - 1), at(row, col), at(row + 1, col + 1));
    std::string word2 = std::format("{}{}{}", at(row + 1, col - 1), at(row, col), at(row - 1, col + 1));
    return (word1 == "MAS" || word1 == "SAM") && (word2 == "MAS" || word2 == "SAM");
  }

  char at(int row, int col) const {
    return row < 0 || row >= std::ssize(grid) || col < 0 || col >= std::ssize(grid[row]) ? ' ' : grid[row][col];
  }

  std::vector<std::string> grid{};
};

std::istream& operator>>(std::istream& is, Grid& grid) {
  std::string line{};
  while (std::getline(is, line)) {
    grid.grid.push_back(std::move(line));
  }
  return is;
}

int main() {
  Grid grid{};
  std::cin >> grid;
  std::println("Part 1 result = {}", grid.xmas(true));
  std::println("Part 2 result = {}", grid.xmas(false));
  return 0;
}
