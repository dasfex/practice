#include <algorithm>
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {

  std::string line;
  uint64_t sum = 0;
  while (getline(std::cin, line)) {
    uint64_t count = stoi(std::string(begin(line) + line.find('\t') + 1, end(line)));
    std::cin >> count;
    sum += count;
  }
  std::cout << std::string(begin(line), begin(line) + line.find('\t')) << '\t' << sum << std::endl;

  return 0;
}
