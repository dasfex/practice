#include <bits/stdc++.h>
#include <cmath>
#include <iostream>
#include <vector>

using ld = long double;
using namespace std;

namespace NFpm {

ld integratePoly(ld a, ld b, const vector<ld>& coefs) {
  vector<ld> res(size(coefs) + 1);
  for (int i = 0; i < size(coefs); ++i) {
    res[i + 1] = coefs[i] / (i + 1);
  }
  ld integral = 0, xPow = b;
  for (int i = 1; i < size(res); ++i) {
    integral += xPow * res[i];
    xPow *= b;
  }
  xPow = a;
  for (int i = 1; i < size(res); ++i) {
    integral -= xPow * res[i];
    xPow *= a;
  }
  return integral;
}

vector<ld> multPol(const vector<ld>& a, const vector<ld>& b) {
  vector<ld> ret(size(a) + size(b) - 1);
  for (int i = 0; i < size(a); ++i) {
    for (int j = 0; j < size(b); ++j) {
      ret[i + j] += a[i] * b[j];
    }
  }
  return ret;
}

ld getAi(ld a, ld b, int i) {
  ld ret = 1;
  for (int j = 0; j < (4 - i) % 2; ++j) {
    ret *= -1;
  }
  for (int div = 1; div <= i; ++div) {
    ret /= div;
  }
  for (int div = 1; div <= 4 - i; ++div) {
    ret /= div;
  }
  ret *= (b - a) / 4;
  vector<ld> coefs{1};
  for (int j = 0; j < 5; ++j) {
    if (i == j) { continue; }
    coefs = multPol(coefs, {ld(-j), 1});
  }
  return ret * integratePoly(0, 4, coefs);
}

ld part(ld a, ld b, const function<ld(ld)>& f) {
  ld sum = 0;
  ld sh = b - a;
  for (int i = 0; i < 5; ++i) {
    sum += getAi(a, b, i) * f(a + i * sh / 4);
  }
  return sum;
}

} // namespace NFpm

void adaptiveFpm(ld a, ld b, ld eps, const function<ld(ld)>& f) {
  ld p = 6;
  ld sh = b - a;
  ld result = 0;
  while (a < b) {
    sh = min(sh, b - a);
    ld i1 = NFpm::part(a, a + sh, f);
    ld i2 = NFpm::part(a, a + sh / 2, f) +
        NFpm::part(a + sh / 2, a + sh, f);
    ld err = abs((i2 - i1) / (pow(2, p) - 1));
    if (err < eps) {
      a += sh;
      result += i2;
    }
    sh = pow(eps / err, 1. / p) * sh / 2;
  }
}

int main() {
  long double a = -5, b = 5, eps = 1e-12;
  auto f = [](long double x) {
    return exp(-x * x);
  };

  adaptiveFpm(a, b, eps, f);

  return 0;
}
