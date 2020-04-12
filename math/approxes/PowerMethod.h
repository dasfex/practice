#ifndef POWER_METHOD_H_
#define POWER_METHOD_H_

#include <deque>
#include <iostream>
#include <vector>
#include <cmath>
#include <complex>
#include <random>

using std::vector;
using std::deque;
using std::istream;
using std::ostream;
using std::cerr;
using std::endl;
using std::complex;

class PowerMethod {
 public:

  explicit PowerMethod(size_t n) : Matrix(n, vector<double>(n)) {
    std::random_device random_device;
    std::uniform_real_distribution<double> interval(-1, 1);
    vector<double> u(n);
    double norm = 0;
    for (auto& element : u) {
      element = interval(random_device);
      norm += element * element;
    }
    norm = std::sqrt(norm);
    for (auto& x : u) {
        x /= norm;
    }
    U.push_back(u);
  }

  [[nodiscard]] size_t Size() const {
    return Matrix.size();
  }

  [[nodiscard]] size_t GetIterations() const {
    return Iter;
  }

  [[nodiscard]] std::pair<vector<complex<double>>, vector<complex<double>>> GetEigenvectors() const {
    return Eigenvectors;
  }

  [[nodiscard]] std::pair<complex<double>, complex<double>> GetAnswer() const {
    return Answer;
  }

  void FindMaxModuloEigenvalue() {
    bool isConverge = false;
    while (!isConverge) {
      isConverge = PowerIteration();
      ++Iter;
    }
  }

  friend istream& operator>>(istream&, PowerMethod&);
  friend ostream& operator<<(ostream&, const PowerMethod&);

 private:
  vector<vector<double>> Matrix;
  deque<vector<double>> U;
  deque<vector<double>> V;
  deque<double> Norm;
  double Rho = 0;
  double CosPhi = 0;
  double Lambda = 0;
  double EPS = 1e-10;
  size_t Iter = 0;
  std::pair<complex<double>, complex<double>> Answer;
  std::pair<vector<complex<double>>, vector<complex<double>>> Eigenvectors;

  bool ConvergenceChecker(double, double, double);
  bool PowerIteration();
};

istream& operator>>(istream& in, PowerMethod& matrix) {
  for (auto& row : matrix.Matrix) {
    for (auto& element : row) {
      in >> element;
    }
  }
  return in;
}

ostream& operator<<(ostream& out, const PowerMethod& matrix) {
  for (const auto& row : matrix.Matrix) {
    for (const auto& element : row) {
      out << element << ' ';
    }
    out << endl;
  }
  return out;
}

bool PowerMethod::ConvergenceChecker(double newRho, double newCosPhi, double newLambda) {
  if (newLambda == Lambda) {
    double secondLambda = 0;
    for (size_t i = 0; i < Size(); ++i) {
      secondLambda += V[3][i] / U[2][i];
    }
    secondLambda /= Size();
    if (std::abs(std::abs(secondLambda) - newLambda) <= EPS) {
      Answer.first.real(secondLambda);
      Eigenvectors.first.resize(Size());
      for (size_t i = 0; i < Size(); ++i) {
        Eigenvectors.first[i].real(U[2][i]);
      }
    } else {
      Answer.first.real(Lambda);
      Answer.second.real(-Lambda);
      Eigenvectors.first.resize(Size());
      Eigenvectors.second.resize(Size());
      for (size_t i = 0; i < Size(); ++i) {
        Eigenvectors.first[i].real(V[3][i] + Lambda * U[2][i]);
        Eigenvectors.second[i].real(V[3][i] - Lambda * U[2][i]);
      }
    }

    return true;
  } else if (newRho == Rho && newCosPhi == CosPhi) {
    double re = Rho * CosPhi;
    double im = std::sqrt(Rho * Rho - re * re);
    Answer.first.real(re);
    Answer.first.imag(im);
    Answer.second.real(re);
    Answer.second.imag(-im);
    Eigenvectors.first.resize(Size());
    Eigenvectors.second.resize(Size());
    for (size_t i = 0; i < Size(); ++i) {
      Eigenvectors.first[i].real(V[3][i] - Answer.second.real() * U[2][i]);
      Eigenvectors.first[i].imag(-Answer.second.imag() * U[2][i]);
      Eigenvectors.second[i].real(V[3][i] - Answer.first.real() * U[2][i]);
      Eigenvectors.second[i].imag(-Answer.first.imag() * U[2][i]);
    }
    return true;
  }

  Rho = newRho;
  CosPhi = newCosPhi;
  Lambda = newLambda;

  return false;
}

bool PowerMethod::PowerIteration() {
  vector<double> newV(Size());
  double maxCoor = -1;
  for (size_t i = 0; i < Size(); ++i) {
    for (size_t j = 0; j < Size(); ++j) {
      newV[i] += Matrix[i][j] * U.back()[j];
    }
    maxCoor = std::max(maxCoor, std::abs(newV[i]));
  }
  bool isUWas = false;
  if (U.size() == 1 && V.empty()) {
    isUWas = true;
  }
  Norm.push_back(maxCoor);
  V.push_back(newV);
  // now newV is newU
  for (auto& element : newV) {
    element /= maxCoor;
  }
  if (isUWas) {
    U.pop_front();
  }
  U.push_back(newV);

  if (U.size() > 4) {
    U.pop_front();
    V.pop_front();
    Norm.pop_front();

    double numerator = 0;
    double denominator = 0;
    for (size_t i = 0; i < Size(); ++i) {
      numerator += V[1][i] * V[3][i] * Norm[2] - V[2][i] * V[2][i] * Norm[1];
      denominator += U[0][i] * V[2][i] - U[1][i] * U[1][i] * Norm[1];
    }

    double newRho = std::sqrt(numerator / denominator);
    double newCosPhi = 0;
    double newLambda = 0;

    for (size_t i = 0; i < Size(); ++i) {
      newCosPhi += (V[3][i] * Norm[2] + numerator / denominator * U[1][i]) / (2 * newRho * V[2][i]);
      newLambda += V[3][i] * Norm[2] / U[1][i];
    }

    newCosPhi /= Size();
    newLambda = std::sqrt(newLambda / Size());

    if (Iter > 0) {
      return ConvergenceChecker(newRho, newCosPhi, newLambda);
    }
  }
  return false;
}

#endif
