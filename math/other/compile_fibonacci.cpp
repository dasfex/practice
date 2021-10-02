#include <iostream>

using namespace std;

// count fibonachi number with matrix binary exponential on compilation
// you can compile with -ftemplate-depth=x 
// when x is the max depth of template recursion

namespace {

constexpr uint64_t M = 1000000007;

template<uint64_t a, uint64_t b, uint64_t c, uint64_t d>
struct matrix {
  static constexpr uint64_t m11 = a % M;
  static constexpr uint64_t m12 = b % M;
  static constexpr uint64_t m21 = c % M;
  static constexpr uint64_t m22 = d % M;
};

template<uint64_t a, uint64_t b, 
         uint64_t c, uint64_t d,
         uint64_t q, uint64_t w, 
         uint64_t e, uint64_t r>
struct matrix_mul {
  static constexpr matrix<(a * q + b * e) % M,
                          (a * w + b * r) % M,
                          (c * q + d * e) % M,
                          (c * w + d * r) % M> val = {};
};

template<size_t N>
struct bin_pow_wrapper;

template<size_t N>
struct fibonachi {
  static constexpr auto val = [] {
    if constexpr (N % 2 == 1) {
      using PrevFib = decltype(fibonachi<N - 1>::val);
      return matrix_mul<
          PrevFib::m11, PrevFib::m12,
          PrevFib::m21, PrevFib::m22,
          1, 1, 1, 0
      >::val;
    } else {
      using HalfFib = decltype(fibonachi<N / 2>::val);
      return matrix_mul<
          HalfFib::m11, HalfFib::m12,
          HalfFib::m21, HalfFib::m22,
          HalfFib::m11, HalfFib::m12,
          HalfFib::m21, HalfFib::m22
      >::val;
    }
  }();
};

template<>
struct fibonachi<1> {
  static constexpr matrix<1, 1, 1, 0> val = {};
};

} // anonymous
  
template<size_t N>
struct Fibonachi {
    static constexpr auto value = decltype(fibonachi<N>::val)::m12;
};

int32_t main() {
  cout << Fibonachi<1000000000000000000>::value << endl;
  
  return 0;
}
