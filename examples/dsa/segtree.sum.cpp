// Problem: n numbers and q queries: "1 i x" adds x to a[i], "2 l r" prints the sum of a[l..r).
// Input:
//   5 4
//   1 2 3 4 5
//   2 0 5
//   1 2 10
//   2 1 3
//   2 3 3
// Output:
//   15
//   15
//   0
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

#include "dsa/segtree.sum.cpp"  // in a solution: <leader>rl -> dsa/segtree -> sum

int main() {
    int n, q;
    cin >> n >> q;
    vector<ll> a(n);
    for (auto &x : a) cin >> x;
    SumSegtree seg(a);
    while (q--) {
        int type;
        ll x, y;
        cin >> type >> x >> y;
        if (type == 1) seg.add((int)x, y);
        else cout << seg.query((int)x, (int)y) << '\n';
    }
}
