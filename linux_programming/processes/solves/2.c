#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

long long it = 0;

void sig_handler(int signo) {
  if (signo == SIGINT)
    printf("Iterations: %lld", it);
  exit(0);
}

int main(void) {
  signal(SIGINT, sig_handler);
  while(1)
    ++it;
  return 0;
}
