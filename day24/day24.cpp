#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <print>
#include <format>
#include <sstream>
#include <algorithm>

using Input = std::map<std::string, bool>;
using Num = uint64_t;

std::string var(char var, Num index) {
  return std::format("{}{:02}", var, index);
}

auto split(std::string line) {
  std::vector<std::string> result{};
  std::string word{};
  std::istringstream stream(std::move(line));
  while (stream >> word) {
    result.push_back(word);
  }
  return result;
}

struct Gate {
  std::string lhs{};
  std::string op{};
  std::string rhs{};
};

class Device {
  friend std::istream& operator>>(std::istream& is, Device& device);
public:
  Num part1() const {
    Num result{};
    for (Num i = 0;; i++) {
      if (const auto out = var('z', i); gates.contains(out)) {
        result |= calculate(out) << i;
      } else {
        return result;
      }
    }
  }

  // Since system is adder, every 'z' output system will follow pattern
  std::string part2() {
    std::vector<std::string> swapped{};

    for (Num i = 5; swapped.size() < 8; i++) {
      verify(swapped, i);
    }

    std::ranges::sort(swapped);
    return std::ranges::fold_left(swapped, std::string{}, [](const auto& a, const auto& b) { return a.empty() ? b : (a + "," + b); });
  }
private:
  std::string findfix(std::vector<std::string>& swapped, const std::string& gate1, const std::string& op, const std::string& gate2) {
    for (const auto& [name, gate] : gates) {
      if (gate.op == op && ((gate.lhs == gate1 && gate.rhs == gate2) || (gate.lhs == gate2 && gate.rhs == gate1))) {
        return name;
      }
    }

    auto found1 = std::ranges::find_if(gates, [gate1, op](const auto& e) { return e.second.op == op && (e.second.lhs == gate1 || e.second.rhs == gate1); });
    auto found2 = std::ranges::find_if(gates, [gate2, op](const auto& e) { return e.second.op == op && (e.second.lhs == gate2 || e.second.rhs == gate2); });
    if (found1 != gates.end()) {
      swapped.push_back(gate2);
      swapped.push_back(found1->second.lhs == gate1 ? found1->second.rhs : found1->second.lhs);
    } else {
      swapped.push_back(gate1);
      swapped.push_back(found2->second.lhs == gate2 ? found2->second.rhs : found2->second.lhs);
    }
    std::swap(gates.at(swapped[swapped.size() - 2]), gates.at(swapped[swapped.size() - 1]));

    return found1 != gates.end() ? found1->first : found2->first;
  }

  void verify(std::vector<std::string>& swapped, Num index) {
    const auto& prev = gates.at(var('z', index - 1));
    const auto current = findfix(swapped, findfix(swapped, findfix(swapped, prev.lhs, "AND", prev.rhs), "OR", findfix(swapped, var('x', index - 1), "AND", var('y', index - 1))), "XOR", findfix(swapped, var('x', index), "XOR", var('y', index)));
    if (const auto output = var('z', index); current != output) {
      std::swap(gates.at(output), gates.at(current));
      swapped.push_back(current);
      swapped.push_back(output);
    }
  }

  Num calculate(const std::string& name) const {
    if (const auto value = input.find(name); value != input.end()) {
      return value->second;
    } else {
      const auto gate = gates.at(name);
      const auto left = calculate(gate.lhs);
      const auto right = calculate(gate.rhs);
      return (gate.op == "AND") ? (left && right) : (gate.op == "OR" ? left || right : left ^ right);
    } 
  }

  std::map<std::string, Gate> gates{};
  Input input{};
};

std::istream& operator>>(std::istream& is, Device& device) {
  std::string line{};
  while (std::getline(is, line)) {
    if (line.empty()) {
      break;
    }
    const auto words = split(std::move(line));
    device.input[words[0].substr(0, words[0].size() - 1)] = std::stoi(words[1]);
  }

  while (std::getline(is, line)) {
    const auto words = split(std::move(line));
    device.gates[words[4]] = Gate{words[0], words[1], words[2]};
  }
  return is;
}

int main() {
  Device device{};
  std::cin >> device;
  std::println("Part 1 result = {}", device.part1());
  std::println("Part 2 result = {}", device.part2());
  return 0;
}
