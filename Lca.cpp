#include <bits/stdc++.h>

using namespace std;

const int N = 1e5 + 7;

int log;
vector<int> d[N];
int up[N][20];
int tin[N], tout[N];

void Dfs(int v, int p = 0) {
    static int timer = 0;
    tin[v] = ++timer;
    up[v][0] = p;
    for (int i = 1; i <= log; ++i) {
        up[v][i] = up[up[v][i - 1]][i - 1];
    }
    for (auto to : d[v]) {
        if (to != p) {
            Dfs(to, v);
        }
    }
    tout[v] = timer;
}

bool IsAncestor(int a, int b) {
    return (tin[a] <= tin[b] && tout[b] <= tout[a]);
}

int Lca(int a, int b) {
    if (IsAncestor(a, b)) return a;
    if (IsAncestor(b, a)) return b;
    for (int i = log; i >= 0; --i) {
        if (!IsAncestor(up[a][i], b)) {
            a = up[a][i];
        }
    }
    return up[a][0];
}

int main() {
    ios_base::sync_with_stdio(false);
    int n;
    for (int i = 0; i < n; ++i) {
        int a, b;
        cin >> a >> b;
        --a; --b;
        d[a].push_back(b);
        d[b].push_back(a);
    }
    log = 1;
    while ((1 << log) < n) {
        ++log;
    }
    Dfs(0);
    int q;
    cin >> q;
    for (int i = 0; i < q; ++i) {
        int a, b;
        cin >> a >> b;
        cout << Lca(a, b) + 1 << endl;
    }

    return 0;
}
