#include <algorithm>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

using namespace std;

struct edge{
  int u;
  int v;
  int w;
};

class Dsu{
 public:
  explicit Dsu(int n) {
    dsu.resize(n + 1, -1);
  }

  int Find(int v) {
    return dsu[v] < 0 ? v : dsu[v] = Find(dsu[v]);
  }

  void Union(int u, int v) {
    u = Find(u);
    v = Find(v);
    if (u > 0 && u == v) return;
    if (dsu[u] > dsu[v]) swap(u, v);
    dsu[u] += dsu[v];
    dsu[v] = u;
  }

 private:
  vector<int> dsu;
};

vector<edge> kruskal(size_t n, const vector<edge>& edges) {
  Dsu dsu(n);
  vector<edge> forest;
  size_t sz = 0;
  for (const edge& e : edges) {
    if (sz == n - 1) { continue; }
    int u = dsu.Find(e.u);
    int v = dsu.Find(e.v);
    if (u == v) { continue; }
    ++sz;
    dsu.Union(e.u, e.v);
    forest.push_back(e);
  }
  return forest;
}

vector<edge> parallelKruskal(size_t n, vector<edge> edges, size_t concurrency = 1) {
  sort(begin(edges), end(edges), [](auto l, auto r) {
    return l.w < r.w;
  });
  size_t m = size(edges);
  vector<edge> combine;
  if (concurrency > 1) {
    size_t blockSize = m / concurrency;
    size_t surplus = m % concurrency;
    vector<vector<edge>> edgesForBlocks(concurrency);
    vector<thread> threads;
    vector<vector<edge>> result;
    mutex mt;
    size_t ind = 0;
    for (auto& efb : edgesForBlocks) {
      for (size_t i = 0; i < blockSize; ++i) {
        efb.push_back(edges[ind++]);
      }
      if (surplus > 0) {
        --surplus;
        efb.push_back(edges[ind++]);
      }
      threads.emplace_back([&efb, &result, &mt, n]() {
        vector<edge> forest = kruskal(n, efb);
        lock_guard<mutex> lock(mt);
        result.push_back(forest);
      });
    }
    for (auto& thread : threads) {
      thread.join();
    }
    for (auto& res : result) {
      copy(begin(res), end(res), back_inserter(combine));
    }
  } else {
    combine = edges;
  }
  return kruskal(n, combine);
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  cout.tie(nullptr);
  size_t n, m;
  cin >> n >> m;
  vector<edge> edges;
  for (size_t i = 0; i < m; ++i) {
    int u, v, w;
    cin >> u >> v >> w;
    edges.push_back({u, v, w});
  }
  auto mst = parallelKruskal(n, edges, 2);
  size_t ans = 0;
  for (auto e : mst) {
    ans += e.w;
  }
  cout << ans;

  return 0;
}

