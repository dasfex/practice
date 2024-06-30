class Dsu {
  std::vector<int> dsu;

public:
  Dsu(int n) : dsu{std::vector<int>(n + 1, -1)} {}

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
};
