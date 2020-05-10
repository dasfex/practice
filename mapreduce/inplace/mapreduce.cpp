#include <boost/process.hpp>

#include <iostream>
#include <string>
#include <utility>
#include <vector>

namespace bp = boost::process;

std::unordered_map<std::string, std::vector<int>>
makeContainer(const std::string& file) {
  std::unordered_map<std::string, std::vector<int>> words;

  std::ifstream fin(file);
  std::string word;
  while (fin >> word) {
    int count;
    fin >> count;
    words[word].push_back(count);
  }
  fin.close();

  return words;
}

void writeValuesToFile(const std::string& file, const std::string& key,
                       const std::vector<int>& values) {
  std::ofstream tmpOut(file);
  for (int value : values) {
    tmpOut << key << '\t' << value << std::endl;
  }
  tmpOut.close();
}

void mergeFiles(const std::string& dstFile, const std::vector<std::string>& files) {
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

size_t getFileLinesCount(const std::string& file) {
  std::ifstream fin(file);
  return std::count(std::istreambuf_iterator<char>(fin),
                    std::istreambuf_iterator<char>(), '\n');;
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

enum class ARGS {
  kCommand = 1,
  kScriptPath,
  kSourceFile,
  kDstFile,
  kMapCount
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

    std::ifstream srcFin(srcFile);
    std::vector<std::pair<std::string, bp::child>> processes;
    std::vector<std::string> dstFiles;
    for (size_t fileInd = 0; fileInd < mapCount; ++fileInd) {
      auto tmpFile = getNewTmpFile(srcFin, blockSize, surplus);
      std::string dstFile = std::tmpnam(nullptr);
      dstFiles.push_back(dstFile);
      processes.emplace_back(tmpFile, bp::child("./" + scriptPath,
          bp::std_out > dstFile, bp::std_in < tmpFile));
    }

    for (auto& [file, process] : processes) {
      process.wait();
      bp::system("rm " + file);
    }

    mergeFiles(dstFile, dstFiles);

  } else if (function == "reduce") {

    auto words = makeContainer(srcFile);

    // sort and shuffle in this task useless cause inplace

    std::vector<std::pair< std::string, bp::child>>
    processes;
    std::vector<std::string> dstFiles;
    for (auto&[key, values] : words) {
      std::string tmpInputName = std::tmpnam(nullptr);
      writeValuesToFile(tmpInputName, key, values);

      std::string tmpDstName = std::tmpnam(nullptr);
      dstFiles.push_back(tmpDstName);

      processes.emplace_back(tmpInputName,
          bp::child("./" + scriptPath, bp::std_out > tmpDstName,
                                       bp::std_in < tmpInputName)
      );
    }
    words.clear();

    for (auto& [file, process] : processes) {
      process.wait();
      bp::system("rm " + file);
    }

    mergeFiles(dstFile, dstFiles);

  } else { // not map and not reduce

    std::cerr << "Wrong output format. Available operations: map, reduce.";
    return 228;
  }

  return 0;
}
