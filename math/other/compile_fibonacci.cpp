#include <iostream>

using namespace std;

// count fibonachi number with matrix binary exponential on compilation
// you can compile with -ftemplate-depth=x 
// when x is the max depth of template recursion

constexpr uint64_t M = 1000000007;

template<uint64_t a, uint64_t b, uint64_t c, uint64_t d>
struct matrix {
  static constexpr uint64_t m11 = a % M;
  static constexpr uint64_t m12 = b % M;
  static constexpr uint64_t m21 = c % M;
  static constexpr uint64_t m22 = d % M;
};

template<uint64_t a, uint64_t b, uint64_t c, uint64_t d,
    uint64_t q, uint64_t w, uint64_t e, uint64_t r>
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
      return matrix_mul<
          decltype(fibonachi<N - 1>::val)::m11, decltype(fibonachi<N - 1>::val)::m12,
          decltype(fibonachi<N - 1>::val)::m21, decltype(fibonachi<N - 1>::val)::m22,
          1, 1, 1, 0
      >::val;
    } else {
      return matrix_mul<
          decltype(bin_pow_wrapper<N>::val)::m11, decltype(bin_pow_wrapper<N>::val)::m12,
          decltype(bin_pow_wrapper<N>::val)::m21, decltype(bin_pow_wrapper<N>::val)::m22,
          decltype(bin_pow_wrapper<N>::val)::m11, decltype(bin_pow_wrapper<N>::val)::m12,
          decltype(bin_pow_wrapper<N>::val)::m21, decltype(bin_pow_wrapper<N>::val)::m22
      >::val;
    }
  }();
};

template<size_t N>
struct bin_pow_wrapper {
  static constexpr auto val = fibonachi<N / 2>::val;
};

template<>
struct bin_pow_wrapper<1> {
  static constexpr matrix<1, 1, 1, 0> val = {};
};

template<>
struct fibonachi<1> {
  static constexpr matrix<1, 1, 1, 0> val = {};
};

int32_t main() {
  cout << decltype(fibonachi<1000000000000000000>::val)::m12 << endl;

  return 0;
}
