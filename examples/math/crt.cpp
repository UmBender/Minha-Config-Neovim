// Problem: smallest x >= 0 with x = r_i (mod m_i) for every i (moduli not necessarily coprime), and the
//   period of the solutions; -1 if there is none. Two systems.
// Input:
//   3
//   2 3
//   3 5
//   2 7
//   2
//   1 4
//   2 6
// Output:
//   23 105
//   -1
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

#include "math/egcd.cpp"  // required by math/crt (the picker inserts it)
#include "math/crt.cpp"   // in a solution: <leader>rl -> math/crt

int main() {
    int k;
    while (cin >> k) {
        vector<ll> rs(k), ms(k);
        for (int i = 0; i < k; i++) cin >> rs[i] >> ms[i];
        auto [r, m] = crt(rs, ms);
        if (m == -1) cout << -1 << '\n';
        else cout << r << ' ' << m << '\n';
    }
}
