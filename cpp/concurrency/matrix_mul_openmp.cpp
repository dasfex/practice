#include <cstring>
#include <chrono>
#include <stdexcept>
#include <string>
#include <iostream>
#include <vector>

#define VERIFY(condition, message)            \
        if (!(condition)) {                   \
          throw std::runtime_error(std::string("File: ") + \
          std::string(__FILE__) + \
          std::string(", line ") +            \
          std::to_string(__LINE__) +          \
          std::string(": ") + message); \
        }

class Matrix {
 public:
  explicit Matrix(size_t size = 1, int val = 0)
      : data_(std::vector<int>(size * size, val))
      , size_(size) {}

  int& operator[](std::pair<size_t, size_t> p) {
    return data_[p.first * size_ + p.second];
  }

  int operator[](std::pair<size_t, size_t> p) const {
    return data_[p.first * size_ + p.second];
  }

  [[nodiscard]] size_t Size() const {
    return size_;
  }

  bool operator==(const Matrix& rhs) const {
    if (size_ != rhs.size_) {
      return false;
    }
    for (size_t i = 0; i < size_; ++i) {
      if (data_[i] != rhs.data_[i]) {
        return false;
      }
    }
    return true;
  }

 private:
  std::vector<int> data_;
  size_t size_;
};

std::istream& operator>>(std::istream& in, Matrix& m) {
  for (size_t i = 0; i < m.Size(); ++i) {
    for (size_t j = 0; j < m.Size(); ++j) {
      in >> m[{i, j}];
    }
  }
  return in;
}

std::ostream& operator<<(std::ostream& out, const Matrix& m) {
  for (size_t i = 0; i < m.Size(); ++i) {
    for (size_t j = 0; j < m.Size(); ++j) {
      out << m[{i, j}] << ' ';
    }
    out << std::endl;
  }
}

struct Helper {
  size_t matrix_size = 1;
  size_t block_size = 1;
  bool outer_par = false;
  bool inner_par = false;
};

Helper parseArgs(int argc, char* argv[]) {
  Helper ret{};
  std::vector<std::string> options{"-s", "-b", "-i", "-o"};
  for (size_t i = 1; i < argc; ++i) {
    for (auto& option : options) {
      if (strcmp(option.c_str(), argv[i]) == 0) {
        if (option == "-b") {
          VERIFY(i + 1 < argc, "Where is block size after -b motherfucker?");
          ret.block_size = std::stoi(argv[i + 1]);
        } else if (option == "-i") {
          ret.inner_par = true;
        } else if (option == "-o") {
          ret.outer_par = true;
        }
      }
    }
  }
  return ret;
}

Matrix mul(const Matrix& a, const Matrix& b, const Helper& info) {
  VERIFY(a.Size() == b.Size(), "Why sizes are not equal stupid?");
  Matrix c(a.Size());
  int i;
  bool inner_par = info.inner_par;
  bool outer_par = info.outer_par;
  #pragma omp parallel for shared(a, b, c, inner_par) private(i) if (outer_par) default(none)
  for (i = 0; i < c.Size(); ++i) {
    int j;
    #pragma omp parallel for shared(i, a, b, c) private(j) if (inner_par) default(none)
    for (j = 0; j < c.Size(); ++j) {
      for (size_t k = 0; k < c.Size(); ++k) {
        c[{i, j}] += a[{i, k}] * b[{k, j}];
      }
    }
  }
  return c;
}

Matrix blockMul(const Matrix& a, const Matrix& b, const Helper& info) {
  VERIFY(a.Size() == b.Size(), "Why sizes are not equal stupid?");
  VERIFY(a.Size() % info.block_size == 0, "Matrix size % block size != 0 u know?");
  Matrix c(a.Size());
  size_t block_cnt = a.Size() / info.block_size;
  size_t i, j;
  size_t block_size = info.block_size;
  bool inner_par = info.inner_par;
  bool outer_par = info.outer_par;
  #pragma omp parallel for shared(block_cnt, block_size, a, b, c, inner_par) private(i) if (outer_par) default(none)
  for (i = 0; i < block_cnt; ++i) {
    #pragma omp parallel for shared(i, block_cnt, block_size, a, b, c) private(j) if (inner_par) default(none)
    for (j = 0; j < block_cnt; ++j) {
      for (size_t k = 0; k < block_cnt; ++k) {
        for (size_t ii = i * block_size; ii < (i + 1) * block_size; ++ii) {
          for (size_t jj = j * block_size; jj < (j + 1) * block_size; ++jj) {
            for (size_t kk = k * block_size; kk < (k + 1) * block_size; ++kk) {
              c[{ii, jj}] += a[{ii, kk}] * b[{kk, jj}];
            }
          }
        }
      }
    }
  }
  return c;
}

template <typename T,
    typename = std::void_t<
        decltype(
            std::chrono::duration<double, std::milli>(std::declval<T>() - std::declval<T>())
        )>>
auto getTime(T start, T end) {
  auto time = std::chrono::duration<double, std::milli>(end - start);
  return time.count();
}

int main(int argc, char* argv[]) {
  auto info = parseArgs(argc, argv);

  std::cin >> info.matrix_size;

  Matrix a(info.matrix_size);
  Matrix b(info.matrix_size);
  std::cin >> a >> b;

  auto first_timepoint = std::chrono::high_resolution_clock::now();
  auto first_res = blockMul(a, b, info);
  auto second_timepoint = std::chrono::high_resolution_clock::now();
  auto second_res = mul(a, b, info);
  auto third_timepoint = std::chrono::high_resolution_clock::now();

  VERIFY(first_res == second_res, "Error!");

  auto block_time = getTime(first_timepoint, second_timepoint);
  std::cout << "Block: " << block_time << std::endl;

  auto trivial_time = getTime(second_timepoint, third_timepoint);
  std::cout << "Trivial: " << trivial_time << std::endl;

  return 0;
}
