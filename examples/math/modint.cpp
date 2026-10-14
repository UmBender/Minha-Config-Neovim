// Problem: first line: a modulus m (1 <= m < 2^31, not necessarily prime). Then q queries "a b":
//   print (a + b) mod 998244353, (a * b) mod 1e9+7, and a / b mod m (or -1 if b has no inverse mod m).
// Input:
//   10
//   3
//   3 7
//   -1 4
//   1000000000000 9
// Output:
//   10 21 9
//   3 1000000003 -1
//   757402656 999937007 0
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

#include "math/modint.cpp"  // in a solution: <leader>rl -> math/modint

using M1 = ModInt<998244353>;   // static moduli
using M2 = ModInt<1000000007>;
using DM = ModInt<0>;           // modulus read from the input

int main() {
    int m, q;
    cin >> m >> q;
    DM::setMod(m);
    while (q--) {
        ll a, b;
        cin >> a >> b;
        DM d = b;
        cout << M1(a) + b << ' ' << M2(a) * b << ' ';
        if (d.inv() * d == 1) cout << DM(a) / d << '\n';
        else cout << -1 << '\n';
    }
}
