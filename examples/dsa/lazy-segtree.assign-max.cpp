// Problem: n numbers and q queries: "1 l r x" sets every a[i] to x in [l, r), "2 l r" prints the maximum of
//   a[l..r).
// Input:
//   5 6
//   5 3 8 1 4
//   2 0 5
//   1 1 4 2
//   2 0 3
//   2 2 5
//   1 0 2 -3
//   2 0 5
// Output:
//   8
//   5
//   4
//   4
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

#include "dsa/lazy-segtree.assign-max.cpp"  // in a solution: <leader>rl -> dsa/lazy-segtree -> assign-max

int main() {
    int n, q;
    cin >> n >> q;
    vector<ll> a(n);
    for (auto &x : a) cin >> x;
    RangeAssignMax seg(a);
    while (q--) {
        int type, l, r;
        cin >> type >> l >> r;
        if (type == 1) {
            ll x;
            cin >> x;
            seg.assign(l, r, x);
        } else {
            cout << seg.max(l, r) << '\n';
        }
    }
}
