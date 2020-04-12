#ifndef ROOTS_H
#define ROOTS_H

#include <iostream>
#include <vector>
#include <random>

using std::cerr;
using std::endl;
using std::vector;

class RootsFinder {
 public:

  explicit RootsFinder(size_t n)
      : Matrix(n, vector<double>(n))
        , S(n, vector<double>(n)) {
    for (size_t i = 0; i < Size(); ++i) {
      S[i][i] = 1;
    }
  }

  void Danilevskiy() {
    size_t rightCellBound = Size();
    for (int i = Size() - 1; i >= 0; --i) {
      int j = i - 1;
      while (j >= 0 && std::abs(Matrix[i][j]) <= EPS) {
        --j;
      }
      if (j < 0) {
        Polynoms.push_back({1});
        for (size_t k = i; k < rightCellBound; ++k) {
          Polynoms.back().push_back(-Matrix[i][k]);
          Normalize(Polynoms.back().back());
        }
        rightCellBound = i;
      } else {
        if (j != i - 1) {
          Matrix[j].swap(Matrix[i - 1]);
          for (size_t k = 0; k < Size(); ++k) {
            std::swap(Matrix[k][j], Matrix[k][i - 1]);
            std::swap(S[k][j], S[k][i - 1]);
          }
        }
        double multiplier = Matrix[i][i - 1];
        for (size_t k = 0; k < Size(); ++k) {
          Matrix[k][i - 1] /= multiplier;
          S[k][i - 1] /= multiplier;
          Matrix[i - 1][k] *= multiplier;
          Normalize(Matrix[k][i - 1]);
          Normalize(Matrix[i - 1][k]);
        }
        for (size_t k = 0; k < rightCellBound; ++k) {
          if (k != i - 1) {
            double mult = Matrix[i][k];
            for (size_t l = 0; l < Size(); ++l) {
              Matrix[l][k] -= Matrix[l][i - 1] * mult;
              S[l][k] -= S[l][i - 1] * mult;
              Matrix[i - 1][l] += Matrix[k][l] * mult;
              Normalize(Matrix[l][k]);
              Normalize(Matrix[i - 1][l]);
            }
          }
        }
      }
    }
    // multiply polynoms
    Coefficients = Polynoms[0];
    for (size_t i = 1; i < Polynoms.size(); ++i) {
      isFindEigenvectors = false;
      vector<double> partResult(Coefficients.size() + Polynoms[i].size() - 1);
      for (size_t j = 0; j < Coefficients.size(); ++j) {
        size_t deg1 = Coefficients.size() - j - 1;
        for (size_t k = 0; k < Polynoms[i].size(); ++k) {
          size_t deg2 = Polynoms[i].size() - k - 1;
          partResult[partResult.size() - deg1 - deg2 - 1] += Coefficients[j] * Polynoms[i][k];
          Normalize(partResult[partResult.size() - deg1 - deg2 - 1]);
        }
      }
      Coefficients = partResult;
    }
  }

  vector<double> FindRealRoots() {
    Roots.clear();
    std::uniform_real_distribution<double> Interval =
        std::uniform_real_distribution<double>(-1'000'000, 1'000'000);
    for (const auto& polynom : Polynoms) {
      vector<double> roots = NewtonFindRoots(polynom, Interval);
      for (auto x : roots) {
        Roots.push_back(x);
      }
    }
    cerr << endl;
    return Roots;
  }

  bool FindEigenvectors() {
    if (!isFindEigenvectors) {
      return false;
    }
    vector<double> eigenvector(Size());
    vector<double> multEigenvector(Size());
    for (auto root : Roots) {
      double rootPower = 1;
      for (size_t i = 0; i < Size(); ++i) {
        eigenvector[Size() - i - 1] = rootPower;
        rootPower *= root;
      }
      for (size_t i = 0; i < Size(); ++i) {
        multEigenvector[i] = 0;
        for (size_t j = 0; j < Size(); ++j) {
          multEigenvector[i] += eigenvector[j] * S[i][j];
        }
        Normalize(multEigenvector[i]);
      }
      Eigenvectors.push_back(multEigenvector);
    }
    return true;
  }

  [[nodiscard]] size_t Size() const {
    return Matrix.size();
  }

  [[nodiscard]] vector<double> GetCoefficients() const {
    return Coefficients;
  }

  [[nodiscard]] vector<double> GetRoots() const {
    return Roots;
  }

  [[nodiscard]] vector<vector<double>> GetEigenvectors() const {
    return Eigenvectors;
  }

  friend std::istream& operator>>(std::istream&, RootsFinder&);
  friend std::ostream& operator<<(std::ostream&, const RootsFinder&);

 private:
  vector<vector<double>> Matrix;
  vector<vector<double>> S;
  vector<vector<double>> Polynoms;
  vector<double> Roots;
  vector<vector<double>> Eigenvectors;
  bool isFindEigenvectors = true;
  vector<double> Coefficients;
  double EPS = 1e-9;
  size_t MAX_ITERATION = 1'000'000;
  size_t MAX_ATTEMPTS = 15;
  std::random_device RandomDevice;

  vector<double> NewtonFindRoots(vector<double> polynom,
                                 std::uniform_real_distribution<double> interval) {
    vector<double> result;
    size_t attemptNumber = 0;
    while (polynom.size() > 1) {
      vector<double> dPolynom = derivativePolynom(polynom);
      double randomPoint;
      bool isFindRoot = false;
      while (attemptNumber < MAX_ATTEMPTS) {
        ++attemptNumber;
        randomPoint = interval(RandomDevice);
        size_t iteration = 0;
        while (iteration < MAX_ITERATION) {
          ++iteration;
          double polynomInPoint = calculatePolynomInPoint(polynom, randomPoint);
          // find root
          if (std::abs(polynomInPoint) <= EPS) {
            break;
          }
          randomPoint -= polynomInPoint / calculatePolynomInPoint(dPolynom, randomPoint);
        }
        double polynomInPoint = calculatePolynomInPoint(polynom, randomPoint);
        if (std::abs(polynomInPoint) <= EPS) {
          result.push_back(randomPoint);
          isFindRoot = true;
          break;
        }
      }
      attemptNumber = 0;
      if (!isFindRoot && std::abs(calculatePolynomInPoint(polynom, randomPoint)) > EPS) {
        break;
      }
      dividePolynom(polynom, randomPoint);
    }
    return result;
  }

  static void dividePolynom(vector<double>& polynom, double point) {
    for (size_t i = 0; i < polynom.size() - 1; ++i) {
      polynom[i + 1] += polynom[i] * point;
    }
    polynom.pop_back();
  }

  static double calculatePolynomInPoint(const vector<double>& polynom, double point) {
    double result = 0;
    double pointPow = 1;
    for (int i = polynom.size() - 1; i >= 0; --i) {
      result += polynom[i] * pointPow;
      pointPow *= point;
    }
    return result;
  }

  static vector<double> derivativePolynom(const vector<double>& polynom) {
    vector<double> result;
    for (size_t i = 0; i < polynom.size() - 1; ++i) {
      result.push_back(polynom[i] * (polynom.size() - 1 - i));
    }
    return result;
  }

  void Normalize(double& x) {
    if (std::abs(x) <= EPS) {
      x = 0;
    }
  }
};

std::istream& operator>>(std::istream& in, SecondTask& matrix) {
  for (size_t i = 0; i < matrix.Size(); ++i) {
    for (size_t j = 0; j < matrix.Size(); ++j) {
      in >> matrix.Matrix[i][j];
    }
  }
  return in;
}

std::ostream& operator<<(std::ostream& out, const SecondTask& matrix) {
  for (size_t i = 0; i < matrix.Size(); ++i) {
    for (size_t j = 0; j < matrix.Size(); ++j) {
      out << matrix.Matrix[i][j] << ' ';
    }
    out << endl;
  }
  return out;
}

#endif // ROOTS_H
