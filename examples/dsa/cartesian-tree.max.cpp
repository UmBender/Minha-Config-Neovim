// Problem: given n numbers, print the parent of each index in the max Cartesian tree (-1 for
//   the root).
// Input:
//   7
//   2 1 4 5 1 3 3
// Output:
//   2 0 3 -1 5 3 5
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

#include "dsa/cartesian-tree.max.cpp"  // in a solution: <leader>rl -> dsa/cartesian-tree -> max

int main() {
    int n;
    cin >> n;
    vector<ll> a(n);
    for (auto &x : a) cin >> x;
    for (int p : maxCartesianTree(a)) cout << p << ' ';
    cout << '\n';
}
