#ifndef TMA_H
#define TMA_H

#include <iostream>
#include <cmath>
#include <vector>

class TMA {
 public:
  FourthTask(size_t n) : Matrix(n, std::vector<double>(3)), B(n) {}

  size_t Size() const {
    return Matrix.size();
  }

  bool Solve();

  friend std::istream& operator>>(std::istream&, FourthTask&);
  friend std::ostream& operator<<(std::ostream&, const FourthTask&);

 private:
  double EPS = 1e-9;
  std::vector<std::vector<double>> Matrix;
  std::vector<double> B;

  bool DoSomething(size_t ind, size_t eq) {
    if (std::abs(Matrix[ind][eq]) < std::abs(Matrix[ind + 1][eq])) {
      Matrix[ind].swap(Matrix[ind + 1]);
      std::swap(B[ind], B[ind + 1]);
    }
    if (std::abs(Matrix[ind][eq]) <= EPS) {
      return false;
    }
    double multiplicator = Matrix[ind + 1][eq] / Matrix[ind][eq];
    for (size_t j = eq; j < 3; ++j) {
      Matrix[ind + 1][j] -= Matrix[ind][j] * multiplicator;
    }
    B[ind + 1] -= B[ind] * multiplicator;
    if (ind < Size() - 3) {
      Matrix[ind + 1].erase(std::begin(Matrix[ind + 1]));
      Matrix[ind + 1].push_back(0);
    }
    return true;
  }

  void DoFirst() {
    int32_t ind = Size() - 1;
    for (int32_t j = ind - 1; j >= std::max(0, ind - 2); --j) {
      if (std::abs(Matrix[ind][j]) <= EPS) {
        continue;
      }
      double mul = Matrix[j][2] / Matrix[ind][2];
      Matrix[j][2] = 0;
      B[j] -= B[ind] * mul;
    }
    ind = Size() - 2;
    if (ind < 1) {
      return;
    }
    if (!(std::abs(Matrix[ind][1]) <= EPS)) {
      double mul = Matrix[ind - 1][1] / Matrix[ind][1];
      Matrix[ind - 1][1] = 0;
      B[ind - 1] -= B[ind] * mul;
    }
    if (ind - 2 >= 0 && !(std::abs(Matrix[ind - 2][2]) <= EPS)) {
      double mul = Matrix[ind - 2][2] / Matrix[ind][1];
      Matrix[ind - 2][2] = 0;
      B[ind - 2] -= B[ind] * mul;
    }
  }
};

bool TMA::Solve() {
  for (size_t i = 0; i < Size() - 2; ++i) {
    if (!DoSomething(i, 0)) {
      return false;
    }
  }
  if (!DoSomething(Size() - 2, 1)) {
    return false;
  }
  DoFirst();
  for (int32_t i = Size() - 3; i > 0; --i) {
    for (int32_t j = 1; j < 3; ++j) {
      if (i - j < 0) {
        break;
      }
      if (std::abs(Matrix[i - j][j]) <= EPS) {
        continue;
      }
      double mul = Matrix[i - j][j] / Matrix[i][0];
      Matrix[i - j][j] = 0;
      B[i - j] -= B[i] * mul;
    }
  }
  for (size_t i = 0; i < Size() - 2; ++i) {
    B[i] /= Matrix[i][0];
  }
  for (int32_t i = 1; i < 3; ++i) {
    B[Size() - i] /= Matrix[Size() - i][3 - i];
  }
  return true;
}

std::istream& operator>>(std::istream& in, TMA& matrix) {
  for (size_t i = 0; i < matrix.Size(); ++i) {
    for (size_t j = 0; j < 3; ++j) {
      in >> matrix.Matrix[i][j];
    }
  }
  for (size_t i = 0; i < matrix.Size(); ++i) {
    in >> matrix.B[i];
  }
   return in;
}

std::ostream& operator<<(std::ostream& out, const TMA& matrix) {
  for (size_t i = 0; i < matrix.Size(); ++i) {
    for (size_t j = 0; j < matrix.Matrix[i].size(); ++j) {
      out << matrix.Matrix[i][j] << ' ';
    }
    out << std::endl;
  }
  for (size_t i = 0; i < matrix.Size(); ++i) {
    out << matrix.B[i] << ' ';
  }
  return out;
}

#endif //TMA_H
