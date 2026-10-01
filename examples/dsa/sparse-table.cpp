// Problem: a static array and q queries [l, r): print the range min, max and gcd, and the
//   bitwise OR (general form, any idempotent operation).
// Input:
//   6 3
//   12 18 6 9 3 24
//   0 2
//   1 4
//   0 6
// Output:
//   12 18 6 30
//   6 18 3 31
//   3 24 3 31
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

#include "dsa/sparse-table.cpp"  // in a solution: <leader>rl -> dsa/sparse-table

int main() {
    int n, q;
    cin >> n >> q;
    vector<ll> a(n);
    for (auto &x : a) cin >> x;

    auto mn = minSparseTable(a);  // presets
    auto mx = maxSparseTable(a);
    auto g = gcdSparseTable(a);
    SparseTable orT(a, [](ll x, ll y) { return x | y; });  // general form: op must be idempotent

    while (q--) {
        int l, r;
        cin >> l >> r;
        cout << mn.query(l, r) << ' ' << mx.query(l, r) << ' ' << g.query(l, r) << ' ' << orT.query(l, r) << '\n';
    }
}
