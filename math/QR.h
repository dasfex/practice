#ifndef QR_H_
#define QR_H_

#include <vector>
#include <iostream>
#include <cmath>
#include <algorithm>
#include <map>
#include <complex>

using std::vector;
using std::istream;
using std::ostream;
using std::cerr;
using std::endl;

class EigenvaluesFinder {
 private:
  auto Multiply(const vector<vector<double>>& H, const vector<vector<double>>& m) {
    vector<vector<double>> ret(Size(), vector<double>(Size()));
    for (size_t i = 0; i < Size(); ++i) {
      for (size_t j = 0; j < Size(); ++j) {
        for (size_t k = 0; k < Size(); ++k) {
          ret[i][j] += H[i][k] * m[k][j];
        }
        Normalize(ret[i][j]);
      }
    }
    return ret;
  }

  struct RotationMatrix{
    double cos;
    double sin;

    size_t i;
    size_t j;

    RotationMatrix(double cos, double sin, size_t i, size_t j) : cos(cos), sin(sin), i(i), j(j) {}
  };

  void Normalize(double& x) {
    if (std::abs(x) <= EPS) {
      x = 0;
    }
  }

 public:

  explicit EigenvaluesFinder(size_t n)
      : Matrix(n, vector<double>(n))
        , Q(n, vector<double>(n)) {
    for (size_t i = 0; i < n; ++i) {
      Q[i][i] = 1;
    }
  }

  [[nodiscard]] size_t Size() const {
    return Matrix.size();
  }

  [[nodiscard]] vector<vector<double>> GetEigenvectors() const {
    return Eigenvectors;
  }

  [[nodiscard]] vector<std::complex<double>> GetEigenvalues() const {
    return Eigenvalues;
  }

  [[nodiscard]] size_t GetIter() const {
    return Iter;
  }

  void QR() {
    Hessenberg();
    Iter = 0;
    while (true) {
      QrIteration();
      ++Iter;
      if (ConvergenceChecker()) {
        break;
      }
    }
    if (isSymmetric) {
      FindEigenvectors();
    }
    FindEigenvalues();
  }

  friend istream& operator>>(istream&, EigenvaluesFinder&);
  friend ostream& operator<<(ostream&, const EigenvaluesFinder&);

 private:
  vector<vector<double>> Matrix;
  const double EPS = 1e-10;
  vector<vector<double>> Eigenvectors;
  vector<vector<double>> Q;
  vector<std::complex<double>> Eigenvalues;
  bool isSymmetric = true;
  bool isNotIntersect = false;
  std::map<size_t, double> eigenvalues;
  size_t Iter = 0;

  void Hessenberg() {
    for (size_t i = 0; i < Size() - 2; ++i) {
      size_t index = i + 1;

      if (std::abs(Matrix[index][i]) <= EPS) {
        for (size_t j = i + 1; j < Size(); ++j) {
          if (std::abs(Matrix[index][j]) > EPS) {
            for (size_t k = 0; k < Size(); ++k) {
              std::swap(Matrix[k][i], Matrix[k][j]);
            }
            Matrix[i].swap(Matrix[j]);
            break;
          }
        }
      }

      vector<double> w(Size() - index);
      double normS = 0;
      for (size_t j = index; j < Size(); ++j) {
        normS += Matrix[j][i] * Matrix[j][i];
      }
      double sum = normS;
      normS = std::sqrt(normS);

      double sign = -Matrix[index][i] / std::abs(Matrix[index][i]);
      double topEl = normS * sign;
      double normW = sum - Matrix[index][i] * Matrix[index][i];
      normW += (Matrix[index][i] - topEl) * (Matrix[index][i] - topEl);
      normW = std::sqrt(normW);

      w[0] = (Matrix[index][i] - topEl) / normW;
      for (int j = index + 1; j < Size(); ++j) {
        w[j - index] = Matrix[j][i] / normW;
      }

      vector<vector<double>> H(Size(), vector<double>(Size()));
      for (size_t j = 0; j < Size(); ++j) {
        H[j][j] = 1;
      }
      for (size_t j = index; j < Size(); ++j) {
        for (size_t k = index; k < Size(); ++k) {
          H[j][k] -= 2 * w[j - index] * w[k - index];
        }
      }

      Matrix = Multiply(H, Matrix);
      Q = Multiply(H, Q);
      Matrix = Multiply(Matrix, H);
    }
  }

  void QrIteration() {
    vector<RotationMatrix> rotations;
    for (size_t i = 0; i < Size() - 1; ++i) {
      size_t j = i + 1;
      if (std::abs(Matrix[j][i]) <= EPS) {
        Matrix[j][i] = 0;
        continue;
      }
      double denominator = std::sqrt(Matrix[i][i] * Matrix[i][i] + Matrix[j][i] * Matrix[j][i]);
      RotationMatrix rotationMatrix(Matrix[i][i] / denominator,
                                    -Matrix[j][i] / denominator, i, j);
      rotations.push_back(rotationMatrix);
      for (size_t k = 0; k < Size(); ++k) {
        double first = Matrix[i][k];
        double second = Matrix[j][k];
        Matrix[i][k] = first * rotationMatrix.cos - second * rotationMatrix.sin;
        Matrix[j][k] = first * rotationMatrix.sin + second * rotationMatrix.cos;
        Normalize(Matrix[i][k]);
        Normalize(Matrix[j][k]);
        first = Q[i][k];
        second = Q[j][k];
        Q[i][k] = first * rotationMatrix.cos - second * rotationMatrix.sin;
        Q[j][k] = first * rotationMatrix.sin + second * rotationMatrix.cos;
        Normalize(Q[i][k]);
        Normalize(Q[j][k]);
      }
    }
    for (auto& rotation : rotations) {
      size_t i = rotation.i;
      size_t j = rotation.j;
      for (size_t k = 0; k < Size(); ++k) {
        double first = Matrix[k][i];
        double second = Matrix[k][j];
        Matrix[k][i] = first * rotation.cos - second * rotation.sin;
        Matrix[k][j] = first * rotation.sin + second * rotation.cos;
        Normalize(Matrix[k][i]);
        Normalize(Matrix[k][j]);
      }
    }
  }

  bool ConvergenceChecker() {
    if (!isNotIntersect) {
      isNotIntersect = true;
      for (size_t i = 1; i < Size() - 1; ++i) {
        if (std::abs(Matrix[i][i - 1]) > EPS && std::abs(Matrix[i + 1][i]) > EPS) {
          isNotIntersect = false;
          break;
        }
      }
    }
    if (isNotIntersect) {
      bool isComplex = true;
      for (size_t i = 0; i < Size() - 1; ++i) {
        if (std::abs(Matrix[i + 1][i]) > EPS) {
          double a = Matrix[i][i];
          double b = Matrix[i][i + 1];
          double c = Matrix[i + 1][i];
          double d = Matrix[i + 1][i + 1];
          double D = d * d + a * a - 2 * a * d + 4 * b * c;
          isComplex &= (D < 0);
          if (!isComplex) {
            break;
          }
        }
      }
      bool isConverge = false;
      if (isComplex && eigenvalues.empty()) {
        for (size_t i = 0; i < Size(); ++i) {
          if (isRealEigenvalue(i)) {
            eigenvalues[i] = Matrix[i][i];
          }
        }
        return false;
      } else if (!eigenvalues.empty()) {
        isConverge = true;
        for (size_t i = 0; i < Size(); ++i) {
          if (eigenvalues.count(i) > 0) {
            isConverge &= std::abs(eigenvalues[i] - Matrix[i][i]) <= EPS;
            eigenvalues[i] = Matrix[i][i];
          }
        }
      }
      return isConverge & isComplex;
    }
    return false;
  }

  bool isRealEigenvalue(size_t i) {
    if (i == 0) {
      return std::abs(Matrix[i + 1][i]) <= EPS;
    } else if (i == Size() - 1) {
      return std::abs(Matrix[i][i - 1]) <= EPS;
    }
    return std::abs(Matrix[i + 1][i]) <= EPS && std::abs(Matrix[i][i - 1]) <= EPS;
  }

  void FindEigenvalues() {
    bool isLastValueReal = true;
    for (size_t i = 0; i < Size() - 1; ++i) {
      if (isRealEigenvalue(i)) {
        Eigenvalues.emplace_back(Matrix[i][i], 0);
      } else {
        if (i == Size() - 2) {
          isLastValueReal = false;
        }
        double a = Matrix[i][i];
        double b = Matrix[i][i + 1];
        double c = Matrix[i + 1][i];
        double d = Matrix[i + 1][i + 1];
        double D = d * d + a * a - 2 * a * d + 4 * b * c;
        std::complex<double> first((a + d) / 2, sqrt(std::abs(D)) / 2);
        std::complex<double> second((a + d) / 2, -sqrt(std::abs(D)) / 2);
        Eigenvalues.emplace_back(first);
        Eigenvalues.emplace_back(second);
        ++i;
      }
    }
    if (isLastValueReal) {
      Eigenvalues.push_back(Matrix.back().back());
    }
  }

  void FindEigenvectors() {
    for (auto& vector : Q) {
      Eigenvectors.push_back(vector);
    }
  }
};

istream& operator>>(istream& in, EigenvaluesFinder& matrix) {
  for (size_t i = 0; i < matrix.Size(); ++i) {
    for (size_t j = 0; j < matrix.Size(); ++j) {
      in >> matrix.Matrix[i][j];
    }
  }
  for (size_t i = 0; i < matrix.Size(); ++i) {
    for (size_t j = 0; j < matrix.Size(); ++j) {
      matrix.isSymmetric = (matrix.Matrix[i][j] == matrix.Matrix[j][i]);
    }
  }
  return in;
}

ostream& operator<<(ostream& out, const EigenvaluesFinder& matrix) {
  for (size_t i = 0; i < matrix.Size(); ++i) {
    for (size_t j = 0; j < matrix.Size(); ++j) {
      out.width(10);
      out << matrix.Matrix[i][j] << ' ';
    }
    out << endl;
  }
  return out;
}

#endif //QR_H_
