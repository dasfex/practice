int BinPow(int a, int n, int m) {
  if (n == 1) {
    return a;
  }
  if (n & 1) {
    return a * BinPow(a, n - 1) % m;
  }
  int b = BinPow(a, n / 2) % m;
  return b * b % m;
}
