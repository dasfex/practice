#include <bits/stdc++.h>
using namespace std;

const int INF = INT_MAX;
const int N = 1e3 + 7;
int a[N], d[4 * N], add[4 * N];

void build(int v, int l, int r) {
    if (l == r) {
        d[v] = a[l];
    }
    else {
        int mid = (l + r) >> 1;
        build(v * 2, l, mid);
        build(v * 2 + 1, mid + 1, r);
        d[v] = d[v * 2] + d[v * 2 + 1];
    }
}

void push(int v, int l, int r) {
    if (add[v] != 0) {
        d[v] += add[v] * (r - l + 1);
        if (l != r) {
            add[v * 2] += add[v];
            add[v * 2 + 1] += add[v];
        }
        add[v] = 0;
    }
}

int get(int v, int l, int r, int ql, int qr) {
    push(v, l, r);
    if (l > qr || r < ql) {
        return 0;
    }
    if (ql <= l && r <= qr) {
        return d[v];
    }
    int mid = (l + r) >> 1;
    return get(v * 2, l, mid, ql, qr) +
            get(v * 2 + 1, mid + 1, r, ql, qr);
}

void updOtr(int v, int l, int r, int ql, int qr, int val) {
    push(v, l, r);
    if (l > qr || r < ql) {
        return;
    }
    if (ql <= l && r <= qr) {
        add[v] = val;
        push(v, l, r);
        return;
    }
    int mid = (l + r) >> 1;
    updOtr(v * 2, l, mid, ql, qr, val);
    updOtr(v * 2 + 1, mid + 1, r, ql, qr, val);
    d[v] = d[v * 2] + d[v * 2 + 1];
}

int main() {
    //freopen("input.txt", "r", stdin);
    int n;
    scanf("%d", &n);
    for (int i = 1; i <= n; ++i){
        scanf("%d", &a[i]);
    }
    build(1, 1, n);
    int q;
    scanf("%d", &q);
    while (q--){
        int type;
        scanf("%d", &type);
        if (type == 1){
            int l, r;
            scanf("%d %d", &l, &r);
            printf("%d\n", get(1, 1, n, l, r));
        }
        else {
            if (type == 2){
                int l, r, val;
                scanf("%d %d %d", &l, &r, &val);
                updOtr(1, 1, n, l, r, val);
            }
        }
    }

    return 0;
}
