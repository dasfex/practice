#include <boost/process.hpp>

#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include "ext_sort.h"

namespace bp = boost::process;

std::vector<std::string> makeContainer(const std::string& file,
                                       const std::string& comparatorPath) {
  std::vector<std::string> words;
  std::ofstream fout;

  std::ifstream fin(file);
  std::string line;
  std::string lastKey;
  while (std::getline(fin, line)) {
    std::stringstream ss(line);
    std::string key;
    ss >> key;
    int isEqual = bp::system("./" + comparatorPath +
        " \"" + lastKey + "\" \"" + key +"\" ");
    if (isEqual == 0) {
      fout.close();
      std::string curName = std::tmpnam(nullptr);
      words.push_back(curName);
      fout.open(curName);
    }
    fout << line << std::endl;
    lastKey = key;
  }
  fout << line;

  return words;
}

void mergeFilesToSource(const std::string& dstFile,
                        const std::vector<std::string>& files) {

  std::ofstream fout(dstFile);
  for (auto& name : files) {
    std::ifstream tmpFileDst(name);
    std::string line;
    while (std::getline(tmpFileDst, line)) {
      fout << line << std::endl;
    }
    tmpFileDst.close();
    bp::system("rm " + name);
  }
  fout.close();
}

std::string getNewTmpFile(std::ifstream& source, size_t blockSize, size_t& surplus) {
  std::string fileName = std::tmpnam(nullptr);
  std::ofstream fout(fileName);
  for (size_t i = 0; i < blockSize + surplus > 0 ? 1 : 0; ++i) {
    std::string line;
    getline(source, line);
    fout << line << std::endl;
  }
  if (surplus > 0) {
    --surplus;
  }
  return fileName;
}

enum class ARGS{
  kCommand = 1,
  kScriptPath,
  kSourceFile,
  kDstFile,
  kMapCount = 5,
  kComparatorPath = 5
};

int main(int argc, char* argv[]) {

  std::string function = argv[static_cast<int>(ARGS::kCommand)];
  std::string scriptPath = argv[static_cast<int>(ARGS::kScriptPath)];
  std::string srcFile = argv[static_cast<int>(ARGS::kSourceFile)];
  std::string dstFile = argv[static_cast<int>(ARGS::kDstFile)];

  if (function == "map") {
    size_t mapCount = std::stoi(argv[static_cast<int>(ARGS::kMapCount)]);
    size_t lines = getFileLinesCount(srcFile);
    size_t blockSize = lines / mapCount;
    size_t surplus = lines % mapCount;
    if (blockSize == 0) {
      mapCount = surplus;
    }

    std::ifstream srcFin(srcFile);
    std::vector<std::pair<std::string, bp::child>> processes;
    std::vector<std::string> dstFiles;
    for (size_t fileInd = 0; fileInd < mapCount; ++fileInd) {
      auto tmpFile = getNewTmpFile(srcFin, blockSize, surplus);
      std::string dstFile = std::tmpnam(nullptr);
      dstFiles.push_back(dstFile);

      processes.emplace_back(tmpFile, bp::child("./" + scriptPath,
                                                bp::std_out > dstFile,
                                                bp::std_in < tmpFile));
    }

    for (auto&[file, process] : processes) {
      process.wait();
      bp::system("rm " + file);
    }

    mergeFilesToSource(dstFile, dstFiles);

  } else if (function == "reduce") {
    std::string comparatorPath = argv[static_cast<int>(ARGS::kComparatorPath)];

    sortFile(srcFile);

    auto words = makeContainer(srcFile, comparatorPath);

    std::vector<std::pair<std::string, bp::child>> processes;
    std::vector<std::string> dstFiles;
    for (auto& inputName : words) {
      std::string tmpDstName = std::tmpnam(nullptr);
      dstFiles.push_back(tmpDstName);

      processes.emplace_back(inputName,
                             bp::child("./" + scriptPath,
                                       bp::std_out > tmpDstName,
                                       bp::std_in < inputName));
    }
    words.clear();

    for (auto&[file, process] : processes) {
      process.wait();
      bp::system("rm " + file);
    }

    mergeFilesToSource(dstFile, dstFiles);

  } else { // not map and not reduce

    std::cerr << "Wrong output format. Available operations: map, reduce.";
    return 228;
  }

  return 0;
}
