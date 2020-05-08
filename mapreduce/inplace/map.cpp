#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>

int main() {
  std::string line;
  while (getline(std::cin, line)) {
    std::stringstream ss;
    ss << std::string(begin(line) + line.find('\t') + 1, end(line));
    std::string word;
    while (ss >> word) {
      std::cout << word << '\t' << 1 << std::endl;
    }
  }

  return 0;
}
