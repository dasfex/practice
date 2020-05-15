namespace NDsu {

struct q {
  int u;
  int v;
  int dsuv;
};

int n;
vector<int> dsu;
stack<q, vector<q>> st;

void Init(int sz) {
  n = sz;
  dsu.resize(n + 1, -1);
}

int Find(int v) {
  return dsu[v] < 0 ? v : Find(dsu[v]);
}

void Union(int u, int v) {
  u = Find(u);
  v = Find(v);
  if (u > 0 && u == v) return;
  if (dsu[u] > dsu[v]) swap(u, v);
  st.push({u, v, dsu[v]});
  dsu[u] += dsu[v];
  dsu[v] = u;
}

void Backup() {
  q b = st.top();
  st.pop();
  dsu[b.v] = b.dsuv;
  dsu[b.u] -= b.dsuv;
}

} // NDsu
