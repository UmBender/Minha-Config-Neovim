// Problem: given polynomials A (n coefficients) and B (m coefficients), print the coefficients of
//   A * B modulo 998244353, then modulo 1e9 + 7 (any modulus: 3 NTT primes + CRT).
// Input:
//   4 3
//   1 2 3 999999999
//   4 5 1000000000
// Output:
//   4 13 1755669 10533893 14045171 714315251
//   4 13 15 999999976 999999946 56
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

#include "dsa/ntt.cpp"  // in a solution: <leader>rl -> dsa/ntt

void print(const vector<ll> &c) {
    for (size_t i = 0; i < c.size(); i++) cout << c[i] << " \n"[i + 1 == c.size()];
}

int main() {
    int n, m;
    cin >> n >> m;
    vector<ll> a(n), b(m);
    for (auto &x : a) cin >> x;
    for (auto &x : b) cin >> x;

    print(nttConv(a, b));               // mod 998244353 (inputs are reduced first)
    print(convMod(a, b, 1000000007));  // any modulus < 2^31
}
