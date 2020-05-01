#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <errno.h>
#include <string.h>

char** GetArgs(char* argv[], int argc, int* curInd) {
  int rhs = *curInd;
  while (rhs != argc && strcmp(argv[rhs], "|") != 0) {
    ++rhs;
  }
  size_t size = rhs - *curInd + 1;
  char** retArgv = malloc(sizeof(char*) * size);
  if (retArgv == NULL) {
    perror("malloc args error");
    return NULL;
  }
  for (int i = 0; i < size - 1; ++i) {
    retArgv[i] = argv[i + *curInd];
  }
  retArgv[size - 1] = NULL;
  *curInd = rhs + 1;
  return retArgv;
}

void closeFds(int** fds, int count, int end, int unclose) {
  for (int i = 0; i < count; ++i) {
    if (i != unclose) {
      close(fds[i][end]);
    }
  }
}

int main(int argc, char* argv[]) {
  if (argc == 1 || strcmp(argv[1], "|") == 0 || strcmp(argv[argc - 1], "|") == 0) {
    fprintf(stderr, "Incorrect args. Call should contain one or more args.\n"
                    "Before first and after last pipeline should be one or more args.\n");
    return 2;
  }

  int pipelineCnt = 0;
  int flag = 0;
  for (int i = 2; i < argc - 1; ++i) {
    if (strcmp(argv[i], "|") == 0) {
      if (flag == 1) {
        fprintf(stderr, "Between two pipelines should be one or more args.\n");
        return 2;
      }
      ++pipelineCnt;
      flag = 1;
    } else {
      flag = 0;
    }
  }

  int** fds;
  if (pipelineCnt > 0) {
    fds = malloc(sizeof(char*) * pipelineCnt);
    if (fds == NULL) {
      perror("fds malloc error");
      return 1;
    }
  }
  int code = 0, fdsInd = 0;
  for (int i = 0; i < pipelineCnt; ++i) {
    fds[i] = malloc(2 * sizeof(int));
    if (fds[i] == NULL) {
      perror("fds[i] malloc error");
      code = 1;
      goto end;
    }
    fdsInd = i;
    if (pipe(fds[i]) == -1) {
      perror("pipe error");
      code = 1;
      goto end;
    }
  }

  int* childsPids = malloc((pipelineCnt + 1) * sizeof(int));
  if (childsPids == NULL) {
    perror("malloc childs pids error");
    code = 1;
    goto end;
  }
  char*** args = malloc(sizeof(char**) * (pipelineCnt + 1));
  if (args == NULL) {
    perror("malloc args error");
    code = 1;
    goto end1;
  }

  int pidsInd = 0, argsInd = 0;
  int argInd = 1;
  for (int i = 0; i <= pipelineCnt; ++i) {
    args[i] = GetArgs(argv, argc, &argInd);
    if (args[i] == NULL) {
      code = 1;
      goto end2;
    }
    argsInd = i;
    int forkPid = fork();
    if (forkPid == -1) {
      perror("fork few processes error");
      code = 1;
      goto end2;
    } else if (forkPid == 0) { // child
      if (i != pipelineCnt) {
        if (dup2(fds[i][1], 1) == -1) {
          perror("dup2 stdout error");
          _exit(1);
        }
        closeFds(fds, pipelineCnt, 1, i);
      } else {
        closeFds(fds, pipelineCnt, 1, -1);
      }
      if (i == 0) {
        closeFds(fds, pipelineCnt, 0, -1);
      } else {
        if (dup2(fds[i - 1][0], 0) == -1) {
          perror("dup2 stdin error");
          _exit(1);
        }
        closeFds(fds, pipelineCnt, 0, i - 1);
      }
      if (execvp(args[i][0], args[i]) == -1) {
        perror("execvp error");
        _exit(1);
      }
    } else { // parent
      childsPids[pidsInd++] = forkPid;
    }
  }

  closeFds(fds, pipelineCnt, 0, -1);
  closeFds(fds, pipelineCnt, 1, -1);

  for (int i = 0; i < pidsInd; ++i) {
    int status;
    int waitPid = waitpid(-1, &status, WUNTRACED);
    if (WIFSTOPPED(status) != 0 || WIFEXITED(status) == 0 ||
        waitPid == -1 || WEXITSTATUS(status) != 0) {
      if (errno != 0) {
        perror("wait error");
      }
      code = 1;
      goto end2;
    }
  }

  end2:
  for (int i = 0; i <= argsInd; ++i) {
    free(args[i]);
  }
  free(args);
  end1:
  free(childsPids);
  end:
  if (pipelineCnt > 0) {
    for (int i = 0; i <= fdsInd; ++i) {
      free(fds[i]);
    }
    free(fds);
  }

  return code;
}
