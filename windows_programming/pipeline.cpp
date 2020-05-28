#include <windows.h>

#include <iostream>
#include <vector>
#include <sstream>
#include <string>

using namespace std;

string GetArgs(char* argv[], int argc, int* curInd) {
  int rhs = *curInd;
  while (rhs != argc && strcmp(argv[rhs], "|") != 0) {
    ++rhs;
  }
  size_t size = rhs - *curInd + 1;
  ostringstream ss;
  for (int i = 0; i + 1 < size; ++i) {
    ss << argv[i + *curInd];
    if (i + 2 < size) {
      ss << ' ';
    }
  }
  ss << '\0';
  *curInd = rhs + 1;
  return ss.str();
}

int main(int argc, char* argv[]) {
  if (argc == 1 || strcmp(argv[1], "|") == 0
      || strcmp(argv[argc - 1], "|") == 0) {
    cerr << "Incorrect args. Call should contain one or more args.\n"
            "Before first and after last pipeline "
            "should be one or more args." << endl;
    return 2;
  }

  int pipelineCnt = 0;
  bool flag = false;
  for (int i = 2; i < argc - 1; ++i) {
    if (strcmp(argv[i], "|") == 0) {
      if (flag) {
        cerr << "Between two pipelines should be one or more args." << endl;
        return 2;
      }
      ++pipelineCnt;
      flag = true;
    } else {
      flag = false;
    }
  }

  vector<vector<HANDLE>> handles(pipelineCnt, vector<HANDLE>(2));
  for (auto& pipe : handles) {
    SECURITY_ATTRIBUTES secAtr;
    secAtr.nLength = sizeof(SECURITY_ATTRIBUTES);
    secAtr.bInheritHandle = TRUE;
    secAtr.lpSecurityDescriptor = NULL;

    if (!CreatePipe(&pipe[0], &pipe[1], &secAtr, 0)) {
      cerr << "Shit happens :) (can't open pipe)" << endl;
      return 1;
    }
  }

  vector<HANDLE> childPids;

  int argsInd = 1;
  for (int i = 0; i <= pipelineCnt; ++i) {
    string args = GetArgs(argv, argc, &argsInd);
    STARTUPINFO startupinfo;
    GetStartupInfo(&startupinfo);
    startupinfo.dwFlags |= STARTF_USESTDHANDLES;
    if (i == 0) {
      startupinfo.hStdInput = GetStdHandle(STD_INPUT_HANDLE);
    } else {
      startupinfo.hStdInput = handles[i - 1][0];
    }
    if (i == pipelineCnt) {
      startupinfo.hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    } else {
      startupinfo.hStdOutput = handles[i][1];
    }
    startupinfo.hStdError = GetStdHandle(STD_ERROR_HANDLE);

    PROCESS_INFORMATION processInfo;
    if (!CreateProcess(
        nullptr,
        args.data(), /* command line */
        nullptr, nullptr,
        true,
        0,
        nullptr, nullptr,
        &startupinfo, &processInfo
    )) {
      cerr << "Failed to create child process." << endl;
      return 1;
    }

    childPids.push_back(processInfo.hProcess);

    if (i != 0) {
      CloseHandle(handles[i - 1][0]);
    } else if (pipelineCnt > 0) {
      CloseHandle(handles[i][1]);
    }
    if (i != pipelineCnt) {
      CloseHandle(handles[i][1]);
    } else if (pipelineCnt > 0) {
      CloseHandle(handles[i][0]);
    }
  }

  int code = 0;
  for (HANDLE& handle : childPids) {
    auto wait_res = WaitForSingleObject(handle, INFINITE);
    if (wait_res == WAIT_FAILED) {
      code = 1;
    }
  }

  return code;
}
