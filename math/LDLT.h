#ifndef LDLT_H_
#define LDLT_H_

#include <cmath>
#include <iostream>
#include <numeric>
#include <vector>

class LDLTClass {
 public:

  explicit LDLTClass(size_t new_size)
      : LDLT(new_size)
      , D(new_size)
      , Permutations(new_size) {
    for (size_t i = 1; i <= new_size; ++i) {
      LDLT[i - 1].resize(i);
    }
    std::iota(std::begin(Permutations), std::end(Permutations), 0);
  }

  [[nodiscard]] size_t Size() const {
    return LDLT.size();
  }

  bool FindLDLT();
  std::vector<double> FindSolutionWithLDLT(std::vector<double>);

  friend std::istream& operator>>(std::istream&, LDLTClass&);
  friend std::ostream& operator<<(std::ostream&, const LDLTClass&);

 private:
  const double EPS = 1e-9;
  std::vector<std::vector<double>> LDLT;
  std::vector<int16_t> D;
  std::vector<size_t> Permutations;
};

std::istream& operator>>(std::istream& in, LDLTClass& matrix) {
  double x;
  for (size_t i = 0; i < matrix.Size(); ++i) {
    for (size_t j = 0; j < i + 1; ++j) {
      in >> matrix.LDLT[i][j];
      matrix.LU[i][j] = matrix.LDLT[i][j];
    }
    for (size_t j = i + 1; j < matrix.Size(); ++j) {
      int x;
      in >> x;
    }
  }
  return in;
}

std::ostream& operator<<(std::ostream& out, const LDLTClass& matrix) {
  for (size_t i = 0; i < matrix.Size(); ++i) {
    for (double j : matrix.LDLT[i]) {
      out << j << ' ';
    }
    out << std::endl;
  }
  for (size_t i = 0; i < matrix.Size(); ++i) {
    out << matrix.D[i] << ' ';
  }
  return out;
}

bool LDLTClass::FindLDLT() {
  for (size_t i = 0; i < Size(); ++i) {
    for (size_t j = i; j < Size(); ++j) {
      double sum = LDLT[j][i];
      for (size_t k = 0; k < i; ++k) {
        sum -= D[k] * LDLT[i][k] * LDLT[j][k];
      }
      if (i == j) {
        if (std::abs(sum) <= EPS) {
          return false;
        }
        D[i] = sum / std::abs(sum);
        LDLT[j][i] = sqrtl(std::abs(sum));
      } else {
        LDLT[j][i] = sum / (D[i] * LDLT[i][i]);
      }
    }
  }
  return true;
}

std::vector<double> LDLTClass::FindSolutionWithLDLT(std::vector<double> b) {
  for (size_t i = 0; i < Size(); ++i) {
    for (size_t j = i + 1; j < Size(); ++j) {
      b[j] -= b[i] * LDLT[j][i] / LDLT[i][i];
    }
    b[i] /= LDLT[i][i];
  }
  for (size_t i = 0; i < Size() - 1; ++i) {
    if (std::abs(D[i] + 1) <= EPS) {
      b[i] = -b[i];
    }
  }
  for (int32_t i = Size() - 1; i >= 0; --i) {
    for (int32_t j = i - 1; j >= 0; --j) {
      b[j] -= b[i] * LDLT[i][j] / LDLT[i][i];
    }
    b[i] /= LDLT[i][i];
  }
  return b;
}

#endif //LDLT_H_
