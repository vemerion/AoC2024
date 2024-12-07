#include <print>
#include <iostream>
#include <string>
#include <regex>
#include <sstream>

std::string parse() {
  std::string result{};
  std::string line{};
  while (std::getline(std::cin, line)) {
    result += line;
  }
  return result;
}

int run(const std::string& program, bool part1) {
  const std::regex instr{"(mul\\((\\d{1,3}),(\\d{1,3})\\))|(do\\(\\))|(don't\\(\\))"};
  int sum{};
  bool enabled = true;
  for (auto itr = std::sregex_iterator(program.begin(), program.end(), instr); itr != std::sregex_iterator(); itr++) {
    const auto str = itr->str();
    if (str.starts_with("mul") && enabled) {
      std::istringstream stream(str);
      int op1{};
      int op2{};
      char unused{};
      stream >> unused >> unused >> unused >> unused >> op1 >> unused >> op2;
      sum += op1 * op2;
    } else if (str.starts_with("do()")) {
      enabled = true;
    } else if (str.starts_with("don't") && !part1) {
      enabled = false;
    }
  }
  return sum;
}

int main() {
  const auto program = parse();
  std::println("Part 1 result = {}", run(program, true));
  std::println("Part 2 result = {}", run(program, false));
  return 0;
}
