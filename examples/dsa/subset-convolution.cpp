// Problem: k jobs; Alice can do the set of jobs T in a[T] ways and Bob in b[T] ways. For every set S,
//   print the number of ways to split S between them (disjoint parts, either may be empty).
// Input:
//   3
//   5 0 1 7 2 0 0 3
//   1 1 2 0 3 0 1 1
// Output:
//   5 5 11 8 17 2 12 29
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

#include "dsa/subset-convolution.cpp"  // in a solution: <leader>rl -> dsa/subset-convolution

int main() {
    int k;
    cin >> k;
    vector<ll> a(1 << k), b(1 << k);
    for (auto &x : a) cin >> x;
    for (auto &x : b) cin >> x;
    vector<ll> c = subsetConvolution(a, b);  // subsetConvolution(a, b, 998244353) for a modulus
    for (size_t s = 0; s < c.size(); s++) cout << c[s] << " \n"[s + 1 == c.size()];
}
