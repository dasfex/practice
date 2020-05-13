#ifndef BASIC_BF_H_
#define BASIC_BF_H_

#include <random>
#include <string>
#include <vector>

namespace std {
string to_string(const string& str) {
  return str;
}
} // namespace std

namespace {

// std::_Hash_impl::hash are implementation defined
template<class T = size_t>
class hasher{
 public:

  hasher() : Seed(std::mt19937{std::random_device{}()}()) {}

  size_t operator()(T x) {
    std::string str = std::to_string(x);
    return std::_Hash_impl::hash(str.data(), std::size(str), Seed);
  }

 private:
  size_t Seed;
};

} // namespace

template<class T>
class counting_bloom_filter{
 public:

  counting_bloom_filter(double failProbability, size_t n, size_t teta)
      : Teta(teta) {
    auto ln2 = std::log(2);
    size_t m = std::ceil(-(n * std::log(failProbability) / ln2 / ln2));
    size_t k;
    if (Teta > 30) {
      k = std::ceil(static_cast<double>(m) / n * (0.2037 * Teta + 0.9176));
    } else {
      k = std::ceil(static_cast<double>(m) / n * ln2);
    }
    Bits.resize(m);
    Hashes.resize(k);
  }

  void add(const T& x) {
    for (auto& hash : Hashes) {
      ++Bits[hash(x) % std::size(Bits)];
    }
  }

  bool check(const T& x) {
    for (auto& hash : Hashes) {
      if (Bits[hash(x) % std::size(Bits)] < Teta) {
        return false;
      }
    }
    return true;
  }

  void clear() {
    std::fill(begin(Bits), end(Bits), 0);
  }

 private:
  size_t Teta;
  std::vector<size_t> Bits;
  std::vector<hasher<T>> Hashes;
};

#endif // BASIC_BF_H_

