// Problem: print the coefficients (constant term first) of the polynomial of degree < n through the n
//   points (x_i, y_i), modulo 998244353.
// Input:
//   3
//   0 1 2
//   1 3 7
// Output:
//   1 1 1
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

#include "math/powm.cpp"                     // required by math/lagrangian-polynomial (the picker inserts it)
#include "math/lagrangian-polynomial.cpp"   // in a solution: <leader>rl -> math/lagrangian-polynomial

int main() {
    int n;
    cin >> n;
    vector<ll> xs(n), ys(n);
    for (auto &x : xs) cin >> x;
    for (auto &y : ys) cin >> y;
    auto c = lagrangePoly(xs, ys);
    for (int i = 0; i < n; i++) cout << c[i] << " \n"[i + 1 == n];
}
