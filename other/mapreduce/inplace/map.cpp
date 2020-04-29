#include <algorithm>
#include <string>
#include <sstream>
#include <iostream>

int main(int argc, char* argv[]) {
  std::string line;
  while (getline(std::cin, line)) {
    std::stringstream ss;
    ss << line;
    bool isWasKey = false;
    std::string word;
    while (ss >> word) {
      if (isWasKey) {
        std::cout << word << '\t' << 1 << std::endl;
      }
      isWasKey = true;
    }
  }

  return 0;
}
