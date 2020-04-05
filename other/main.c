#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define SIZE 1000

int code = 0;

void func(int fd) {
  char* s = malloc((SIZE + 1) * sizeof(char));
  while (1) {
    int size = read(fd, s, SIZE);
    if (size == -1) {
      perror("ERROR");
      code = 1;
      break;
    }
    s[size] = 0;
    write(1, s, size);
    if (size < SIZE) {
      break;
    }
  }
  free(s);
}

int main(int argc, char* argv[]) {
  int fd = 0;
  if (argc > 1) {
    for (int i = 1; i < argc; ++i) {
      fd = open(argv[i], O_RDONLY);
      if (fd == -1) {
        perror("err");
        code = 1;
        continue;
      }

      func(fd);
    }
  } else {
    func(fd);
  }

  return code;
}
