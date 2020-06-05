#include <bits/stdc++.h>

#include <cmath>
#include <iostream>
#include <iomanip>
#include <vector>

using ld = long double;
using namespace std;

namespace NGaussThree {

vector<ld> xis{-sqrt(0.6), 0, sqrt(0.6)};
vector<ld> w{5/9., 8/9., 5/9.};

ld masterElement(ld a, ld b, ld xi) {
  return (b - a) * (xi + 1) / 2 + a;
}

ld part(ld a, ld b, const function<ld(ld)>& f) {
  ld sum = 0;
  for (int i = 0; i < 3; ++i) {
    sum += w[i] * f(masterElement(a, b, xis[i]));
  }
  return (b - a) / 2 * sum;
}

} // namespace NGaussThree

void adaptiveGt(ld a, ld b, ld eps, const function<ld(ld)>& f) {
  ld p = 6;
  ld sh = b - a;
  ld result = 0;
  while (a < b) {
    sh = min(sh, b - a);
    ld i1 = NGaussThree::part(a, a + sh, f);
    ld i2 = NGaussThree::part(a, a + sh / 2, f) +
        NGaussThree::part(a + sh / 2, a + sh, f);
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

  adaptiveGt(a, b, eps, f);

  return 0;
}
