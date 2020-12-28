#include <algorithm>
#include <cmath>
#include <functional>
#include <iostream>
#include <random>
#include <utility>
#include <vector>

using std::cin;
using std::cout;
using std::vector;

vector<vector<int>> df;
vector<vector<int>> ds;

vector<size_t> hf;
vector<size_t> hs;

vector<size_t> nfg(100001);

void Gen() {
  std::mt19937 gen(std::random_device { } ());
  for (size_t& x : nfg) {
    x = gen();
  }
}

vector<int> Centroid(const vector<vector<int>>& dd) {
  int nn = dd.size() - 1;
  vector<int> centroid;
  vector<int> sz(nn + 1);
  std::function<void(int, int)> dfs = [&](int uu, int prev) {
    sz[uu] = 1;
    bool is_centroid = true;
    for (auto to : dd[uu])
      if (to != prev) {
        dfs(to, uu);
        sz[uu] += sz[to];
        if (sz[to] > nn / 2) {
          is_centroid = false;
        }
      }
    if (nn - sz[uu] > nn / 2) {
      is_centroid = false;
    }
    if (is_centroid) {
      centroid.push_back(uu);
    }
  };
  dfs(1, 0);
  return centroid;
}

void CalcHash(int vv, vector<vector<int>>& dd, vector<size_t>& hash, int dp = 1, int pr = 0) {
  for (auto to : dd[vv]) {
    if (to == pr) { continue; }
    CalcHash(to, dd, hash, dp + 1, vv);
    hash[vv] *= nfg[dp] + hash[to];
    hash[vv] %= 313216813373;
  }
}

bool Check(int fc, int sc) {
  std::fill(hf.begin(), hf.end(), 1);
  std::fill(hs.begin(), hs.end(), 1);
  CalcHash(fc, df, hf);
  CalcHash(sc, ds, hs);
  return hf[fc] == hs[sc];
}

void MakeDfs(int uu, int vv, vector<int>& res, int pu = 0, int pv = 0) {
  vector<std::pair<int, size_t>> lf;
  vector<std::pair<int, size_t>> ls;
  for (auto to : df[uu]) {
    if (to == pu) { continue; }
    lf.emplace_back(to, hf[to]);
  }
  for (auto to : ds[vv]) {
    if (to == pv) { continue; }
    ls.emplace_back(to, hs[to]);
  }
  std::sort(lf.begin(), lf.end(), [](auto lhs, auto rhs) {
    return lhs.second < rhs.second;
  });
  std::sort(ls.begin(), ls.end(), [](auto lhs, auto rhs) {
    return lhs.second < rhs.second;
  });
  for (size_t i = 0; i < lf.size(); ++i) {
    res[lf[i].first] = ls[i].first;
    MakeDfs(lf[i].first, ls[i].first, res, uu, vv);
  }
}

vector<int> Make(int fc, int sc) {
  vector<int> res(df.size());
  res[fc] = sc;
  MakeDfs(fc, sc, res);
  return res;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  Gen();
  int nn;
  cin >> nn;
  df.resize(nn + 1);
  ds.resize(nn + 1);
  hf.resize(nn + 1);
  hs.resize(nn + 1);
  for (int i = 0; i < nn - 1; ++i) {
    int uv, vv;
    cin >> uv >> vv;
    df[uv].push_back(vv);
    df[vv].push_back(uv);
  }
  auto cf = Centroid(df);
  for (int i = 0; i < nn - 1; ++i) {
    int uv, vv;
    cin >> uv >> vv;
    ds[uv].push_back(vv);
    ds[vv].push_back(uv);
  }
  auto cs = Centroid(ds);
  for (auto fc : cf) {
    for (auto sc : cs) {
      if (Check(fc, sc)) {
        auto res = Make(fc, sc);
        for (size_t i = 1; i < res.size(); ++i) {
          cout << res[i] << '\n';
        }
        return 0;
      }
    }
  }
  cout << -1;

  return 0;
}
