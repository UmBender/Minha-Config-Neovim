// Problem: given a permutation of 1..n, print its 1-based position in lexicographic order and the
//   permutation k positions after it (wrapping around).
// Input:
//   4
//   3 1 4 2
//   12
// Output:
//   14
//   1 2 4 3
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

#include "math/perm-int.cpp"  // in a solution: <leader>rl -> math/perm-int

int main() {
    int n;
    ll k, total = 1;
    cin >> n;
    vector<int> p(n);
    for (auto &x : p) cin >> x, x--;  // 0-based values
    cin >> k;
    for (int i = 2; i <= n; i++) total *= i;
    ll r = perm2int(p);
    cout << r + 1 << '\n';
    auto q = int2perm(n, (r + k) % total);
    for (int i = 0; i < n; i++) cout << q[i] + 1 << " \n"[i + 1 == n];
}
