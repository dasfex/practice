#include <stdio.h>
#include <memory.h>

int main() {
  for (int i = 0; i < 10 * 1024; ++i) {
    malloc(1048576 * 1024);
  }
  char n;
  scanf("%c", &n);

  return 0;
}
