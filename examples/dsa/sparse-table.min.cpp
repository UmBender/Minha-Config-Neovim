// Problem: a static array and q queries [l, r): print the minimum of a[l..r).
// Input:
//   6 3
//   12 18 6 9 3 24
//   0 2
//   1 4
//   0 6
// Output:
//   12
//   6
//   3
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

#include "dsa/sparse-table.min.cpp"  // in a solution: <leader>rl -> dsa/sparse-table -> min

int main() {
    int n, q;
    cin >> n >> q;
    vector<ll> a(n);
    for (auto &x : a) cin >> x;
    MinSparseTable st(a);
    while (q--) {
        int l, r;
        cin >> l >> r;
        cout << st.query(l, r) << '\n';
    }
}
