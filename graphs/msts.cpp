#include <bits/stdc++.h>

#define int long long
#define endl '\n'

using namespace std;

class Dsu{
  struct q{
    int u;
    int v;
    int dsuv;
  };

  int n;
  vector<int> dsu;

 public:
  Dsu(int sz) {
    n = sz;
    dsu.resize(n + 1, -1);
  }

  int Find(int v) {
    return dsu[v] < 0 ? v : dsu[v] = Find(dsu[v]);
  }

  bool isTogether(int u, int v) {
    u = Find(u);
    v = Find(v);
    return u == v && u > 0;
  }

  void Union(int u, int v) {
    u = Find(u);
    v = Find(v);
    if (u > 0 && u == v) return;
    if (dsu[u] > dsu[v]) swap(u, v);
    dsu[u] += dsu[v];
    dsu[v] = u;
  }

};

struct edge{
  int u;
  int v;
  int w;
  int i;
};

int n, m, mst = -228;

enum class EdgeState{
  good, neutral, bad,
};

vector<vector<int>> ans;

pair<int, vector<int>> getMST(const vector<edge>& edges,
                              const vector<EdgeState>& state) {
  Dsu dsu(n);
  vector<int> taken;
  taken.reserve(n - 1);
  int sz = 0, anss = 0;
  vector<int> good;
  for (int i = 0; i < m; ++i) {
    if (state[i] == EdgeState::good) {
      ++sz;
      anss += edges[i].w;
      dsu.Union(edges[i].u, edges[i].v);
      good.push_back(i);
    }
  }
  for (int i = 0; i < m; ++i) {
    if (state[i] != EdgeState::neutral) continue;
    if (dsu.isTogether(edges[i].u, edges[i].v)) continue;
    taken.push_back(i);
    ++sz;
    anss += edges[i].w;
    dsu.Union(edges[i].u, edges[i].v);
  }
  if (sz != n - 1) {
    return {numeric_limits<int>::max(), {}};
  }
  if (anss == mst || mst == -228) {
    good.insert(end(good), begin(taken), end(taken));
    ans.push_back(move(good));
    return {anss, taken};
  }
  return {numeric_limits<int>::max(), {}};
}

void getMsts(const vector<edge>& edges, vector<EdgeState> state,
             const vector<int>& taken) {
  for (int i = 0; i < size(taken); ++i) {
    state[taken[i]] = EdgeState::bad;
    auto[cur, tk] = getMST(edges, state);
    if (cur == mst) {
      getMsts(edges, state, tk);
    }
    state[taken[i]] = EdgeState::good;
  }
}

int32_t main() {
  freopen("input.txt", "r", stdin);
  freopen("output.txt", "w", stdout);
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> n >> m;
  vector<edge> edges;
  edges.reserve(m);
  for (int i = 0; i < m; ++i) {
    int u, v, w;
    cin >> u >> v >> w;
    edges.push_back({u, v, w, i});
  }

  sort(begin(edges), end(edges), [](auto l, auto r) {
    return l.w < r.w;
  });

  vector<EdgeState> state(m, EdgeState::neutral);
  vector<int> taken;
  tie(mst, taken) = getMST(edges, state);
  getMsts(edges, state, taken);

  cout << ans.size() << endl;
  for (auto& v : ans) {
    for (int x : v) {
      cout << edges[x].i + 1 << ' ';
    }
    cout << endl;
  }

  return 0;
}
