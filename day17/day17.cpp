#include <iostream>
#include <print>
#include <regex>
#include <vector>
#include <algorithm>

using Num = uint64_t;

std::vector<Num> findNumbers(std::string& s) {
  std::vector<Num> result{};
  std::regex regex{"(\\d+)"};
  auto numbers = std::sregex_iterator(s.begin(), s.end(), regex);
  for (auto itr = numbers; itr != std::sregex_iterator(); itr++) {
    result.push_back(std::stoi(itr->str()));
  }
  return result;
}

class Computer {
  friend std::istream& operator>>(std::istream& is, Computer& computer);
public:
  // Derived from staring at input for too long
  Num part2() const {
    std::vector<std::vector<Num>> as{};
    as.push_back({0});
    /*
      Every loop find 3 bits for register 'a',
      but since there are many possible answers we need
      keep them in vector 'as'
    */
    for (int i = std::ssize(program) - 1; i >= 0; i--) {
      const auto out = program[i];
      as.push_back({});
      for (const auto a : as[as.size() - 2]) {
        for (Num b = 0; b < 8; b++) {
          const Num c = ((a << 3) + b) / (1 << (b ^ 1));
          if ((((b ^ 1) ^ 4) ^ c) % 8 == out) {
            as.back().push_back((a << 3) + b);
          }
        }
      }
    }
    return *std::ranges::min_element(as.back());
  }

  std::string part1() {
    std::string result{};
    while (pc < program.size()) {
      const auto instr = program[pc];
      const auto operand = program[pc + 1];
      switch (instr) {
        case 0: // adv
          regA = regA / (1 << combo(operand));
          pc += 2;
          break;
        case 1: // bxl
          regB = regB ^ operand;
          pc += 2;
          break;
        case 2: // bst
          regB = combo(operand) % 8;
          pc += 2;
          break;
        case 3: // jnz
          pc = (regA == 0) ? (pc + 2) : operand;
          break;
        case 4: // bxc
          regB = regB ^ regC;
          pc += 2;
          break;
        case 5: { // out
          const auto value = std::to_string(combo(operand) % 8);
          result += (result.empty() ? "" : ",") + value;
          pc += 2;
          break;
        }
        case 6: // bdv
          regB = regA / (1 << combo(operand));
          pc += 2;
          break;
        case 7: // cdv
          regC = regA / (1 << combo(operand));
          pc += 2;
          break;
      }
    }
    return result;
  }
private:
  Num combo(Num operand) {
    return operand < 4 ? operand : at(operand - 4);
  }

  Num& at(Num i) {
    return i == 0 ? regA : (i == 1 ? regB : regC);
  }

  Num regA{};
  Num regB{};
  Num regC{};
  Num pc{};
  std::vector<Num> program{};
};

std::istream& operator>>(std::istream& is, Computer& computer) {
  std::string line{};
  for (size_t i = 0; i < 3; i++) {
    std::getline(is, line);
    computer.at(i) = findNumbers(line)[0];
  }
  std::getline(is, line);
  std::getline(is, line);
  computer.program = findNumbers(line);
  return is;
}

int main() {
  Computer computer{};
  std::cin >> computer;
  std::println("Part 1 result = {}", computer.part1());
  std::println("Part 2 result = {}", computer.part2());
}
