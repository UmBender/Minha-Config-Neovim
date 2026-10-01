// Problem: n numbers and q queries: "1 i x" sets a[i] = x, "2 l r" prints the maximum of a[l..r).
// Input:
//   5 3
//   5 3 8 1 4
//   2 0 5
//   1 3 9
//   2 1 4
// Output:
//   8
//   9
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

#include "dsa/segtree.max.cpp"  // in a solution: <leader>rl -> dsa/segtree -> max

int main() {
    int n, q;
    cin >> n >> q;
    vector<ll> a(n);
    for (auto &x : a) cin >> x;
    MaxSegtree seg(a);
    while (q--) {
        int type;
        ll x, y;
        cin >> type >> x >> y;
        if (type == 1) seg.set((int)x, y);
        else cout << seg.query((int)x, (int)y) << '\n';
    }
}
