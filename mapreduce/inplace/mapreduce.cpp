#include <boost/process.hpp>

#include <iostream>
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
    std::string word;
    while (tmpFileDst >> word) {
      uint64_t count;
      tmpFileDst >> count;
      fout << word << ' ' << count << std::endl;
    }
    tmpFileDst.close();
    bp::system("rm " + name);
  }
  fout.close();
}

int main(int argc, char* argv[]) {

  std::string function = argv[1];
  std::string scriptPath = argv[2];
  std::string srcFile = argv[3];
  std::string dstFile = argv[4];

  if (function == "map") {

    bp::system("./" + scriptPath, bp::std_out > dstFile, bp::std_in < srcFile);

  } else if (function == "reduce") {

    auto words = makeContainer(srcFile);

    // sort and shuffle in this task useless cause inplace

    std::vector<std::string> dstFiles;
    for (auto& [key, values] : words) {
      std::string tmpReduceName = std::tmpnam(nullptr);
      writeValuesToFile(tmpReduceName, key, values);

      std::string tmpDstName = std::tmpnam(nullptr);
      dstFiles.push_back(tmpDstName);

      bp::system("./" + scriptPath, bp::std_out > tmpDstName,
                 bp::std_in < tmpReduceName);
      bp::system("rm " + tmpReduceName);
    }
    words.clear();

    mergeFiles(dstFile, dstFiles);

  } else { // not map and not reduce

    std::cerr << "Wrong output format. Available operations: map, reduce.";
    return 228;
  }

  return 0;
}
