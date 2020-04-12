#ifndef GAUSS_H_
#define GAUSS_H_

#include<vector>
#include<cmath>
#include<iostream>


//  find inverse matrix by gauss with choice in column
class Gauss {
 public:

  Gauss(size_t new_size)
      : Matrix(new_size, std::vector<double>(new_size))
      , Identical(new_size, std::vector<double>(new_size)) {
    for (size_t i = 0; i < new_size; ++i) {
      Identical[i][i] = 1;
    }
  }

  bool MakeGauss() {
    for (size_t i = 0; i < Size() - 1; ++i) {
      size_t max_element_ind = i;
      for (size_t j = i + 1; j < Size(); ++j) {
        if (std::abs(Matrix[max_element_ind][i]) < std::abs(Matrix[j][i])) {
          max_element_ind = j;
        }
      }
      if (std::abs(Matrix[max_element_ind][i]) <= 1e-9) {
        return false;
      }
      double main_element = Matrix[max_element_ind][i];
      if (max_element_ind != i) {
        Matrix[i].swap(Matrix[max_element_ind]);
        Identical[i].swap(Identical[max_element_ind]);
      }
      for (size_t j = i + 1; j < Size(); ++j) {
        if (std::abs(Matrix[j][i]) <= 1e-9) {
          continue;
        }
        double multiplier = Matrix[j][i] / main_element;
        for (size_t k = 0; k < Size(); ++k) {
          Identical[j][k] -= multiplier * Identical[i][k];
        }
        for (size_t k = i; k < Size(); ++k) {
          Matrix[j][k] -= multiplier * Matrix[i][k];
        }
      }
    }
    // обратный ход
    for (size_t i = Size() - 1; i > 0; --i) {
      double main_element = Matrix[i][i];
      for (size_t k = 0; k < i; ++k) {
        if (std::abs(Matrix[k][i]) <= 1e-9) {
          continue;
        }
        double multiplier = Matrix[k][i] / main_element;
        for (size_t j = 0; j < Size(); ++j) {
          if (j > k) {
            Matrix[k][j] -= multiplier * Matrix[i][j];
          }
          Identical[k][j] -= multiplier * Identical[i][j];
        }
      }
    }
    for (size_t i = 0; i < Size(); ++i) {
      for (size_t j = 0; j < Size(); ++j) {
        Identical[i][j] /= Matrix[i][i];
      }
      Matrix[i][i] = 1;
    }
    return true;
  }

  size_t Size() const {
    return Matrix.size();
  }

  auto GetInverse() const {
    return Identical;
  }

  friend std::istream& operator>>(std::istream&, Gauss&);
  friend std::ostream& operator<<(std::ostream&, const Gauss&);

 private:
  std::vector<std::vector<double>> Matrix;
  std::vector<std::vector<double>> Identical;
};

std::istream& operator>>(std::istream& in, Gauss& matrix) {
  for (size_t i = 0; i < matrix.Size(); ++i) {
    for (size_t j = 0; j < matrix.Size(); ++j) {
      int new_element;
      in >> new_element;
      matrix.Matrix[i][j] = static_cast<double>(new_element);
    }
  }
  return in;
}

std::ostream& operator<<(std::ostream& out, const Gauss& matrix) {
  for (size_t i = 0; i < matrix.Size(); ++i) {
    for (size_t j = 0; j < matrix.Size(); ++j) {
      out << matrix.Identical[i][j] << ' ';
    }
    out << std::endl;
  }
  return out;
}

#endif //GAUSS_H_
