#include <algorithm>
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
  std::string curWord;
  uint64_t sum = 0;
  while (std::cin >> curWord) {
    uint64_t count;
    std::cin >> count;
    sum += count;
  }

  std::cout << curWord << '\t' << sum << std::endl;

  return 0;
}
