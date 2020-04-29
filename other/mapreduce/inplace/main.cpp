#include <boost/process.hpp>

#include <algorithm>
#include <iostream>
#include <sstream>
#include <utility>
#include <vector>
#include <random>

namespace bp = boost::process;

std::string GetRandomName() {
  static std::random_device rnd;
  static std::mt19937 gen(rnd());
  std::string correctSymbols
      = "0123456789qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM_";
  std::string name;
  for (int i = 0; i < 100; ++i) {
    name += correctSymbols[gen() % correctSymbols.size()];
  }
  name += ".txt";
  return name;
}

int main(int argc, char* argv[]) {

  std::string function = argv[1];
  std::string scriptPath = argv[2];
  std::string srcFile = argv[3];
  std::string dstFile = argv[4];

  if (function == "map") {
    bp::system("./" + scriptPath, bp::std_out > dstFile, bp::std_in < srcFile);
  } else if (function == "reduce") {
    std::vector<std::pair<std::string, int>> words;
    std::ifstream fin(srcFile);
    std::string word;
    while (fin >> word) {
      int count;
      fin >> count;
      words.emplace_back(word, count);
    }
    fin.close();

    sort(begin(words), end(words));

    auto iter = begin(words);
    std::vector<std::string> reduceFiles;
    std::vector<std::string> dstFiles;
    while (iter != end(words)) {
      std::string tmpReduceName = GetRandomName();
      std::string tmpDstName = GetRandomName();
      reduceFiles.push_back(tmpReduceName);
      dstFiles.push_back(tmpDstName);
      std::ofstream tmpOut(tmpReduceName);
      if (iter == begin(words) || (*(iter - 1)).first != (*iter).first) {
        tmpOut << iter->first << ' ' << iter->second << std::endl;
        ++iter;
      }
      while (iter != end(words) && (*(iter - 1)).first == (*iter).first) {
        tmpOut << iter->first << ' ' << iter->second << std::endl;
        ++iter;
      }
      bp::system("./" + scriptPath, bp::std_out > tmpDstName,
                 bp::std_in < tmpReduceName);
      tmpOut.close();
    }
    words.clear();

    for (auto& name : reduceFiles) {
      bp::system("rm " + name);
    }
    reduceFiles.clear();

    std::ofstream fout(dstFile);
    for (auto& name : dstFiles) {
      std::ifstream tmpFileDst(name);
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

  return 0;
}
