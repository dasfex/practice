#include <bits/stdc++.h>
#define int long long

using namespace std;

const int N = 3e5 + 7, /*MOD = 1e9 + 7,*/ INF = numeric_limits<int>::max();
const long double PI = acos(-1);
const double EPS = 0.000000001;
int m;

template <typename T>
T BinPow(T a, int n) {
    if (n == 1) {
        return a;
    }
    if (n & 1) {
        return a * BinPow(a, n - 1);
    }
    T b = BinPow(a, n / 2);
    return b * b;
}

class Fibonachi {
public:
    Fibonachi() : m_({{1, 1}, {1, 0}}) {}

    Fibonachi operator*(const Fibonachi& t) {
        vector<vector<int>> a(2, vector<int>(2));
        for (int i = 0; i < 2; ++i) {
            for (int j = 0; j < 2; ++j) {
                for (int k = 0; k < 2; ++k) {
                    a[i][j] += ((m_[i][k] % m) * (t.m_[k][j] % m)) % m;
                    a[i][j] %= m;
                }
            }
        }
        m_ = a;
        return *this;
    }

    Fibonachi operator%(const int& n) {
        for (int i = 0; i < 2; ++i) {
            for (int j = 0; j < 2; ++j) {
                m_[i][j] %= n;
            }
        }
        return *this;
    }

    int operator[](const pair<int, int>& i) {
        return m_[i.first][i.second];
    }

private:
    vector<vector<int>> m_;
};

signed main() {
    int n;
    cin >> n;
    cin >> m;
    Fibonachi t;
    t = BinPow(t, n);
    cout << t[{0, 1}];

    return 0;
}
