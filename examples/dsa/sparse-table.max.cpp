// Problem: a static array and q queries [l, r): print the maximum of a[l..r).
// Input:
//   6 3
//   12 18 6 9 3 24
//   0 2
//   1 4
//   0 6
// Output:
//   18
//   18
//   24
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

#include "dsa/sparse-table.max.cpp"  // in a solution: <leader>rl -> dsa/sparse-table -> max

int main() {
    int n, q;
    cin >> n >> q;
    vector<ll> a(n);
    for (auto &x : a) cin >> x;
    MaxSparseTable st(a);
    while (q--) {
        int l, r;
        cin >> l >> r;
        cout << st.query(l, r) << '\n';
    }
}
