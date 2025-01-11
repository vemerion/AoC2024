#include <iostream>
#include <string>
#include <print>
#include <vector>
#include <cassert>

using Columns = std::vector<unsigned>;
using Keys = std::vector<Columns>;
using Locks = std::vector<Columns>;

class Installation {
  friend std::istream& operator>>(std::istream& is, Installation& installation);
public:
  unsigned compatibles() const {
    unsigned count{};
    for (const auto& lock : locks) {
      for (const auto& key : keys) {
        count += compatible(key, lock);
      }
    }

    return count;
  }

private:
  bool compatible(const Columns& key, const Columns& lock) const {
    assert(key.size() == lock.size());

    for (unsigned i = 0; i < key.size(); i++) {
      if (key[i] + lock[i] > max) {
        return false;
      }
    }
    return true;
  }

  Keys keys{};
  Locks locks{};
  unsigned max{};
};

std::istream& operator>>(std::istream& is, Installation& installation) {
  std::string line{};
  std::vector<Columns>* current = nullptr;
  while (std::getline(is, line)) {
    if (line.empty()) {
      if (!is.eof()) {
        installation.max = 0;
      }
      current = nullptr;
      continue;
    }

    installation.max++;
    if (current == nullptr) {
      current = line.contains('#') ? &installation.locks : &installation.keys;
      current->push_back(std::vector(line.size(), 0u));
    }

    for (unsigned i = 0; i < line.size(); i++) {
      current->back()[i] += line[i] == '#';
    }
  }
  return is;
}


int main() {
  Installation installation{};
  std::cin >> installation;
  std::println("Part 1 result = {}", installation.compatibles());
  return 0;
}
