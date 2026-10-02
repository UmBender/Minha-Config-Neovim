// Problem: a tree with a value on each vertex; queries "1 u v" print the sum of the values on the
//   path u -> v and the path read as a number (digits in order u -> v), "2 v x" set the value of v.
// Input:
//   7 5
//   1 2 3 4 5 6 7
//   0 1
//   0 2
//   1 3
//   1 4
//   2 5
//   5 6
//   1 3 6
//   1 6 3
//   2 0 9
//   1 4 4
//   1 4 2
// Output:
//   23 421367
//   23 763124
//   5 5
//   19 5293
#include <bits/stdc++.h>
using namespace std;

#include "tree/hld.cpp"  // in a solution: <leader>rl -> tree/hld

// one segment tree node: the digits of a range as a number, read left to right
struct Num {
    long long sum = 0, val = 0, pw = 1;  // pw = 10^(digits)
};
Num combine(Num a, Num b) { return {a.sum + b.sum, a.val * b.pw + b.val, a.pw * b.pw}; }

// iterative segment tree; rev = true combines the elements in reversed order
struct Seg {
    int n;
    bool rev;
    vector<Num> t;
    Seg(int n_, bool rev_) : n(n_), rev(rev_), t(2 * n_) {}
    Num op(Num a, Num b) { return rev ? combine(b, a) : combine(a, b); }
    void set(int i, long long x) {
        for (t[i += n] = {x, x, 10}; i >>= 1;) t[i] = op(t[2 * i], t[2 * i + 1]);
    }
    Num query(int l, int r) {  // [l, r) in the segment tree's order
        Num a, b;
        for (l += n, r += n; l < r; l >>= 1, r >>= 1) {
            if (l & 1) a = op(a, t[l++]);
            if (r & 1) b = op(t[--r], b);
        }
        return op(a, b);
    }
};

int main() {
    int n, q;
    cin >> n >> q;
    vector<long long> a(n);
    for (auto &x : a) cin >> x;
    vector<vector<int>> g(n);
    for (int i = 0; i + 1 < n; i++) {
        int u, v;
        cin >> u >> v;
        g[u].push_back(v), g[v].push_back(u);
    }
    HLD h(g);
    Seg seg(n, false), rseg(n, true);
    for (int v = 0; v < n; v++) seg.set(h.pos[v], a[v]), rseg.set(h.pos[v], a[v]);
    while (q--) {
        int type;
        cin >> type;
        if (type == 1) {
            int u, v;
            cin >> u >> v;
            Num res;
            for (auto [l, r, up] : h.path(u, v)) res = combine(res, up ? rseg.query(l, r) : seg.query(l, r));
            cout << res.sum << ' ' << res.val << '\n';
        } else {
            int v;
            long long x;
            cin >> v >> x;
            seg.set(h.pos[v], x), rseg.set(h.pos[v], x);
        }
    }
}
