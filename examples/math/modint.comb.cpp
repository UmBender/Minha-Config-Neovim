// Problem: number of lattice paths from (0, 0) to (x, y) with unit steps right/up, modulo 1e9+7
//   (C(x + y, x)), and the number of ways to seat k of n people in a row (P(n, k)).
// Input:
//   3
//   2 2 5 2
//   3 1 10 0
//   100000 100000 4 5
// Output:
//   6 20
//   4 1
//   879467333 0
#include <bits/stdc++.h>
using namespace std;

#include "math/modint.cpp"       // in a solution: <leader>rl -> math/modint -> comb
#include "math/modint.comb.cpp"  // (inserts math/modint too)

using Mint = ModInt<1000000007>;

int main() {
    Comb<Mint> cb;  // grows on demand
    int q;
    cin >> q;
    while (q--) {
        int x, y, n, k;
        cin >> x >> y >> n >> k;
        cout << cb.C(x + y, x) << ' ' << cb.P(n, k) << '\n';
    }
}
