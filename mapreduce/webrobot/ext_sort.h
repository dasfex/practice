#ifndef EXT_SORT_H_
#define EXT_SORT_H_

#include <boost/process.hpp>

#include <algorithm>
#include <fstream>
#include <queue>
#include <string>
#include <utility>
#include <vector>

size_t getFileLinesCount(const std::string& file) {
  std::ifstream fin(file);
  return std::count(std::istreambuf_iterator<char>(fin),
                    std::istreambuf_iterator<char>(), '\n');;
}

bool isFileEmpty(std::ifstream& file) {
  return file.peek() == std::ifstream::traits_type::eof();
}

namespace NUtils {

const size_t kBlockSize = 10000;
size_t linesCount;

void splitFile(const std::string& file, const std::vector<std::string>& names) {
  std::ifstream fin(file);
  for (size_t i = 0, j = 0; i < linesCount; ++j) {
    std::vector<std::string> lines;
    for (size_t k = 0; k < kBlockSize && i < linesCount; ++k, ++i) {
      std::string line;
      getline(fin, line);
      lines.push_back(line);
    }

    std::sort(begin(lines), end(lines));

    std::ofstream fout(names[j]);
    for (auto& line : lines) {
      fout << line << std::endl;
    }
  }
  fin.close();
  std::ofstream fout(file);
  fout << "";
}

void merge(std::vector<std::ifstream>& fins, std::ofstream& fout) {
  using type = std::pair<std::string, size_t>;
  std::priority_queue<type, std::vector<type>, std::greater<type>> queue;
  for (size_t i = 0; i < size(fins); ++i) {
    std::string line;
    getline(fins[i], line);
    queue.push(std::make_pair(line, i));
  }
  std::vector<bool> isClosed(size(fins), false);
  while (!queue.empty()) {
    auto [line, ind] = queue.top();
    queue.pop();
    fout << line << std::endl;
    if (isFileEmpty(fins[ind])) {
      isClosed[ind] = true;
      continue;
    }
    getline(fins[ind], line);
    queue.push(std::make_pair(line, ind));
  }
}

void mergeFiles(const std::string& fileName,
    const std::vector<std::string>& names) {

  std::vector<std::ifstream> ifstreams(size(names));
  for (size_t i = 0; i < size(ifstreams); ++i) {
    ifstreams[i].open(names[i]);
  }
  std::ofstream fout(fileName);
  merge(ifstreams, fout);
}

} // namespace

void sortFile(const std::string& fileName) {
  NUtils::linesCount = getFileLinesCount(fileName);
  std::vector<std::string> names(
      (NUtils::linesCount + NUtils::kBlockSize - 1) / NUtils::kBlockSize);
  for (auto& name : names) {
    name = std::tmpnam(nullptr);
  }
  NUtils::splitFile(fileName, names);
  NUtils::mergeFiles(fileName, names);
  for (auto& name : names) {
    boost::process::system("rm " + name);
  }
}

#endif // EXT_SORT_H_
