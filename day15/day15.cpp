#include <iostream>
#include <string>
#include <map>
#include <print>
#include <algorithm>
#include <ranges>
#include <functional>
#include <vector>
#include <array>

struct Pos {
  int row{};
  int col{};

  auto operator<=>(const Pos&) const = default;

  Pos operator+(const Pos& other) const {
    return {row + other.row, col + other.col};
  }

  bool vertical() const {
    return row != 0;
  }
};

class Warehouse {
  friend std::istream& operator>>(std::istream& is, Warehouse& warehouse);
public:
  int robot() {
    Pos pos = std::ranges::find_if(objects, [](const auto& e) { return e.second == '@'; })->first;

    for (size_t i = 0; i < moves.size(); i++) {
      const auto& direction = directions.at(moves.at(i));
      if (move(pos, direction, false)) {
        pos = pos + direction;
      }
    }

    return std::ranges::fold_left(std::views::keys(objects | std::views::filter([](const auto& e) { return e.second == 'O' || e.second == '['; })) | std::views::transform([](const auto& p) { return 100 * p.row + p.col; }), 0, std::plus());
  }

  Warehouse widen() const {
    Warehouse result{};
    result.moves = moves;

    for (const auto& [p, c] : objects) {
      if (c == '#') {
        result.objects[{p.row, p.col * 2}] = '#';
        result.objects[{p.row, p.col * 2 + 1}] = '#';
      } else if (c == 'O') {
        result.objects[{p.row, p.col * 2}] = '[';
        result.objects[{p.row, p.col * 2 + 1}] = ']';
      } else if (c == '@') {
        result.objects[{p.row, p.col * 2}] = '@';
      }
    }

    return result;
  }

private:
  bool move(Pos pos, const Pos& direction, bool simulated) {
    if (!objects.contains(pos)) {
      return true;
    }

    const char c = objects[pos];
    if (c == '#') { // Can't move walls
      return false;
    } else if (direction.vertical() && (c == '[' || c == ']')) { // Moves big boxes vertically
      const std::array positions{Pos{pos.row, pos.col + (c == '[' ? 1 : -1)}, pos};
      bool ok = true;
      for (const auto& p : positions) {
        if (!move(p + direction, direction, true)) {
          ok = false;
          break;
        }
      }
      if (ok && !simulated) {
        for (const auto& p : positions) {
          move(p + direction, direction, simulated);
          objects[p + direction] = objects[p];
          objects.erase(p);
        }
      }
      return ok;
    } else { // Regular move
      const auto next = pos + direction;
      if (move(next, direction, simulated)) {
        if (!simulated) {
          objects[next] = c;
          objects.erase(pos);
        }
        return true;
      }
    }

    return false;
  }
  inline static const std::map<char, Pos> directions{{'^', Pos{-1, 0}}, {'>', Pos{0, 1}}, {'v', Pos{1, 0}}, {'<', Pos{0, -1}}};

  std::map<Pos, char> objects{};
  std::string moves{};
};

std::istream& operator>>(std::istream& is, Warehouse& warehouse) {
  std::string line{};
  int row{};
  while (std::getline(is, line)) {
    if (line.empty()) {
      break;
    }
    for (int col = 0; col < std::ssize(line); col++) {
      const auto c = line[col];
      if (c != '.') {
        warehouse.objects[{row, col}] = c;
      }
    }
    row++;
  }

  while (std::getline(is, line)) {
    warehouse.moves += line;
  }
  return is;
}

int main() {
  Warehouse warehouse{};
  std::cin >> warehouse;
  Warehouse second = warehouse.widen();
  std::println("Part 1 result = {}", warehouse.robot());
  std::println("Part 2 result = {}", second.robot());
  return 0;
}
