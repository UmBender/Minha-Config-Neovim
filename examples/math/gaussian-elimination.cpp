// Problem: an n x m system A x = b modulo 998244353: print the rank of A, one solution (or -1), and
//   the determinant of the n x n block of A's first columns.
// Input:
//   3 3
//   1 2 3 6
//   2 4 6 12
//   1 0 1 2
// Output:
//   2
//   2 2 0
//   0
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

#include "math/powm.cpp"                  // required by math/gaussian-elimination (the picker inserts it)
#include "math/gaussian-elimination.cpp"  // in a solution: <leader>rl -> math/gaussian-elimination

int main() {
    int n, m;
    cin >> n >> m;
    vector<vector<ll>> a(n, vector<ll>(m));
    vector<ll> b(n);
    for (int i = 0; i < n; i++) {
        for (auto &x : a[i]) cin >> x;
        cin >> b[i];
    }
    auto r = a;
    cout << gaussMod(r) << '\n';
    if (auto x = solveMod(a, b)) {
        for (int j = 0; j < m; j++) cout << (*x)[j] << " \n"[j + 1 == m];
    } else {
        cout << -1 << '\n';
    }
    cout << detMod(a) << '\n';
}
