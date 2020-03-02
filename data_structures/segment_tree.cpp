#include <bits/stdc++.h>
   
#define int long long
   
using namespace std;
   
const int N = 1e5 + 7, MOD = 1e9 + 7, INF = numeric_limits<int>::max();
   
int a[N], d[2 * N];
   
void build(int v, int l, int r) {
    if (l == r) {
        d[v] = a[l];
    } else {
        int mid = (l + r) >> 1;
        build(v + 1, l, mid);
        build(v + (mid - l + 1) * 2, mid + 1, r);
        d[v] = d[v + 1] + d[v + (mid - l + 1) * 2];
    }
}
   
int get(int v, int l, int r, int ql, int qr) {
    if (r < ql || qr < l) {
        return 0;
    }
    if (ql <= l && r <= qr) {
        return d[v];
    }
    int mid = (l + r) >> 1;
    return get(v + 1, l, mid, ql, qr) +
            get(v + (mid - l + 1) * 2, mid + 1, r, ql, qr);
}
   
void upd(int v, int l, int r, int pos, int val) {
    if (l == r) {
        d[v] = val;
    } else {
        int mid = (l + r) >> 1;
        if (pos <= mid) {
            upd(v + 1, l, mid, pos, val);
        } else {
            upd(v + (mid - l + 1) * 2, mid + 1, r, pos, val);
        }
        d[v] = d[v + 1] + d[v + (mid - l + 1) * 2];
    }
}
   
int32_t main() {
  ios_base::sync_with_stdio(false); cin.tie(0);
  int n;
  cin >> n;
  for (int i = 1; i <= n; ++i) {
    cin >> a[i];
  }
  build(1, 1, n);
  int q;
  cin >> q;
  while (q--) {
    char type;
    cin >> type;
    if (type == 'u') {
        int pos, val;
        cin >> pos >> val;
        upd(1, 1, n, pos, val);
    } else {
        int l, r;
        cin >> l >> r;
        cout << get(1, 1, n, l, r) << ' ';
    }
  }
   
  return 0;
}
