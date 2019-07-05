#include <bits/stdc++.h>

using namespace std;

vector<int> min_div(10000001, 1000001);

template <typename T>
T BinPow(T a, int n) {
    if (n == 0) {
        return 1;
    }
    if (n & 1) {
        return a * BinPow(a, n - 1);
    }
    T b = BinPow(a, n / 2);
    return b * b;
}

//  number types
template <typename T>
void Eratosphen(T n) {
  for (int k = 2; k <= n; ++k) {
    for (int i = k; i <= n; i += k) {
        min_div[i] = min(min_div[i], k);
    }
  }
}

long long EilerFunction(long long n) {
  if (min_div[n] == n) {
    return n - 1;
  }
  long long result = 1;
  unordered_map<long long, long long> divs_multiplicity;
  divs_multiplicity.max_load_factor(0.25);
  while (n > 1) {
    ++divs_multiplicity[min_div[n]];
    n /= min_div[n];
  }
  for (auto divisor: divs_multiplicity) {
    long long pow = BinPow(divisor.first, divisor.second);
    result *= (pow - pow / divisor.first);
  }
  return result;
}

int main() {
  Eratosphen(1'000'000);
  int q;
  cin >> q;
  while (q--) {
    int n;
    cin >> n;
    cout << EilerFunction(n) << endl;
  }

  return 0;
}
