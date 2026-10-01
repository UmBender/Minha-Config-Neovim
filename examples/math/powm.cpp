// Problem: for each query a b c (1 <= a < p) print a^(b^c) mod 1e9+7 (Fermat: the exponent is taken mod p - 1),
//   then the modular inverse of a.
// Input:
//   3
//   3 7 1
//   15 2 2
//   2 0 5
// Output:
//   2187 333333336
//   50625 466666670
//   1 500000004
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

#include "math/powm.cpp"  // in a solution: <leader>rl -> math/powm

int main() {
    const ll P = 1000000007;
    int q;
    cin >> q;
    while (q--) {
        ll a, b, c;
        cin >> a >> b >> c;
        ll e = powMod(b, c, P - 1);  // a^(p-1) = 1 since p does not divide a
        cout << powMod(a, e, P) << ' ' << powMod(a, P - 2, P) << '\n';
    }
}
