// Problem: n numbers and q queries: "1 l r x" adds x to a[l..r), "2 i x" adds x to a[i] only,
//   "3 l r" prints the sum of a[l..r).
// Input:
//   5 5
//   1 2 3 4 5
//   3 0 5
//   1 1 4 10
//   3 0 2
//   2 4 100
//   3 3 5
// Output:
//   15
//   13
//   119
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

#include "dsa/fenwick-tree.cpp"  // in a solution: <leader>rl -> dsa/fenwick-tree

int main() {
    int n, q;
    cin >> n >> q;
    vector<ll> a(n);
    for (auto &x : a) cin >> x;

    RangeFenwick rf(a);  // preset: range add + range sum (long long)
    // point updates only? the plain form is enough: Fenwick fw(n); fw.add(i, x); fw.sum(l, r);

    while (q--) {
        int type;
        cin >> type;
        if (type == 1) {
            int l, r;
            ll x;
            cin >> l >> r >> x;
            rf.add(l, r, x);
        } else if (type == 2) {
            int i;
            ll x;
            cin >> i >> x;
            rf.add(i, i + 1, x);
        } else {
            int l, r;
            cin >> l >> r;
            cout << rf.sum(l, r) << '\n';
        }
    }
}
