#ifndef SECONDTASK_H_
#define SECONDTASK_H_

#include<cmath>
#include<vector>

class LU {
 public:

  explicit LU(size_t new_size)
      : Matrix(new_size), L(new_size), permutations(0) {
    for (size_t i = 0; i < new_size; ++i) {
      Matrix[i] = new double[new_size];
      L[i] = new double[i + 1];
      L[i][i] = 1;
    }
  }

  bool FindLU();

  [[nodiscard]] size_t Size() const {
    return Matrix.size();
  }

  [[nodiscard]] auto GetL() const {
    return L;
  }

  [[nodiscard]] auto GetU() const {
    return Matrix;
  }

  friend std::istream& operator>>(std::istream& in, SecondTask& matrix);

 private:
  const double EPS = 1e-9;
  std::vector<double*> Matrix;
  std::vector<double*> L;
  std::vector<std::pair<size_t, size_t>> permutations;
};

bool LU::FindLU() {
  for (size_t i = 0; i < Size(); ++i) {
    size_t max_element_ind = i;
    for (size_t j = i + 1; j < Size(); ++j) {
      if (std::abs(Matrix[max_element_ind][i]) < std::abs(Matrix[j][i])) {
        max_element_ind = j;
      }
    }
    if (std::abs(Matrix[max_element_ind][i]) <= EPS) {
      return false;
    }
    double main_element = Matrix[max_element_ind][i];
    if (max_element_ind != i) {
      permutations.emplace_back(i, max_element_ind);
      std::swap(Matrix[max_element_ind], Matrix[i]);
    }
    for (size_t j = i + 1; j < Size(); ++j) {
      L[j][i] = Matrix[j][i] / Matrix[i][i];
      for (size_t k = 0; k < Size(); ++k) {
        Matrix[j][k] -= L[j][i] * Matrix[i][k];
      }
    }
  }
}

std::istream& operator>>(std::istream& in, SecondTask& matrix) {
  for (size_t i = 0; i < matrix.Size(); ++i) {
    for (size_t j = 0; j < matrix.Size(); ++j) {
      in >> matrix.Matrix[i][j];
    }
  }
  return in;
}

#endif //SECONDTASK_H_
