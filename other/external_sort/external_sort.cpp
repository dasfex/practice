#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <random>
#include <algorithm>
#include <chrono>
#include <queue>

using namespace std;

const int64_t B = 35000;
const auto sz = sizeof(int64_t);
const string INPUT = "input.bin";
const string OUTPUT = "output.bin";

vector<int64_t> BUF;

int64_t N, K;

class Buffer{
 public:
  explicit Buffer(string name)
      : BufSize(0)
        , Name(name) {
    BUF.reserve(B);
    ofstream fout(name);
    fout.close();
  }

  void write(int64_t x, bool append = true) {
    if (BufSize + 1 > B) {
      flush(append);
    }
    BUF.push_back(x);
    ++BufSize;
  }

  void flush(bool append = true) {
    if (BufSize == 0) { return; }
    ofstream fout;
    if (append) {
      fout.open(Name, ios::binary | ios::app);
    } else {
      fout.open(Name, ios::binary);
    }
    for (size_t i = 0; i < BufSize; ++i) {
      fout.write(reinterpret_cast<char*>(&BUF[i]), sz);
    }
    fout.close();
    BufSize = 0;
    BUF.resize(0);
    BUF.reserve(B);
  }

  ~Buffer() {
    flush();
  }

 private:
  size_t BufSize;
  string Name;
};

string getRandomName(size_t size = 10) {
  static random_device rd;
  static mt19937 gen(rd());
  string str = "qwertyuiopasdfghjklzxcvbnm";
  string ret;
  for (size_t i = 0; i < size; ++i) {
    ret += str[gen() % str.size()];
  }
  return ret;
}

void getN() {
  ifstream fin(INPUT, ios::binary);
  fin.read(reinterpret_cast<char*>(&N), sz);
  K = (N + B - 1) / B;
  fin.close();
}

bool isFileEmpty(ifstream& file) {
  return file.peek() == std::ifstream::traits_type::eof();
}

void splitFile(const vector<string>& names) {
  ifstream fin(INPUT, ios::binary);
  static bool isFirstly = false;
  if (!isFirstly) {
    fin.read(reinterpret_cast<char*>(&N), sz);
    isFirstly = true;
  }
  for (size_t i = 0, j = 0; i < N; ++j) {
    Buffer buffer(names[j]);
    for (size_t k = 0; k < B && i < N; ++k, ++i) {
      int64_t x;
      fin.read(reinterpret_cast<char*>(&x), sz);
      buffer.write(x);
    }
    sort(begin(BUF), end(BUF));
  }
  fin.close();
  ofstream fout(INPUT);
  fout << "";
  fout.close();
}

bool mergePhase(vector<ifstream>& files, Buffer& output) {
  using type = pair<int64_t, size_t>;
  BUF.reserve(B);
  priority_queue<type, vector<type>, greater<type>> q;
  for (size_t j = 0; j < K; ++j) {
    int64_t x;
    files[j].read(reinterpret_cast<char*>(&x), sz);
    q.push(make_pair(x, j));
  }
  vector<bool> isClosed(K, false);
  while (!q.empty()) {
    auto [val, ind] = q.top();
    q.pop();
    output.write(val);
    if (isFileEmpty(files[ind])) {
      isClosed[ind] = true;
      continue;
    }
    int64_t x;
    files[ind].read(reinterpret_cast<char*>(&x), sz);
    q.push(make_pair(x, ind));
  }
}

void mergeStage(const vector<string>& names) {
  vector<ifstream> files(K);
  for (size_t i = 0; i < K; ++i) {
    files[i].open(names[i], ios::binary);
  }
  Buffer fout(INPUT);
  mergePhase(files, fout);
  fout.flush();
  for (auto& file : files) {
    file.close();
  }
}

int main() {
  getN();
  vector<string> names(K);
  for (size_t i = 0; i < K; ++i) {
    names[i] = getRandomName(5);
  }

  splitFile(names);
  mergeStage(names);

  ifstream fin(INPUT, ios::binary);
  Buffer fout(OUTPUT);
  fout.write(N);
  for (size_t i = 0; i < N; ++i) {
    int64_t x;
    fin.read(reinterpret_cast<char*>(&x), sz);
    fout.write(x);
  }
  fin.close();
  fout.flush();

  return 0;
}
