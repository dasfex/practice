#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>

int main() {
  int pid = fork();
  if (pid < 0) {
    perror("ERROR");
    return 0;
  } else if (pid > 0) {
    printf("[parent] My PID is %d\n", getpid());
    printf("[parent] Child's PID is %d\n", pid);
    int* stat = malloc(sizeof(int));
    waitpid(pid, stat, NULL);
    printf("[parent] Child exited with code %d\n", WEXITSTATUS(*stat));
    free(stat);
  } else {
    printf("[child] Hello, my PID is %d\n", getpid());
    printf("[child] Bye\n");
    exit(42);
  }

  return 0;
}
