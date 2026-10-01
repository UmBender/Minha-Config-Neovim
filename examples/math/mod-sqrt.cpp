// Problem: for each query a p (p prime) print the smallest x >= 0 with x^2 = a (mod p), or -1.
// Input:
//   5
//   0 5
//   1 2
//   2 7
//   3 7
//   4 998244353
// Output:
//   0
//   1
//   3
//   -1
//   2
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

#include "math/powm.cpp"      // required by math/mod-sqrt (the picker inserts it)
#include "math/mod-sqrt.cpp"  // in a solution: <leader>rl -> math/mod-sqrt

int main() {
    int q;
    cin >> q;
    while (q--) {
        ll a, p;
        cin >> a >> p;
        cout << sqrtMod(a, p) << '\n';
    }
}
