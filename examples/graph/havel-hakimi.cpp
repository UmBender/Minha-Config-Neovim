// Problem: Given the wanted degree of each vertex, print a simple graph with those degrees
//   (edge count, then edges) or NO.
// Input:
//   5
//   3 2 2 2 1
// Output:
//   5
//   0 3
//   0 2
//   0 1
//   4 3
//   2 1
//   NO
#include <bits/stdc++.h>
using namespace std;

#include "graph/havel-hakimi.cpp"  // in a solution: <leader>rl -> graph/havel-hakimi

int main() {
    int n;
    cin >> n;
    vector<int> d(n);
    for (auto &x : d) cin >> x;
    auto res = havelHakimi(d);
    if (!res) {
        cout << "NO\n";
        return 0;
    }
    cout << res->size() << '\n';
    for (auto [u, v] : *res) cout << u << ' ' << v << '\n';
    cout << (havelHakimi({3, 3, 1, 1}) ? "YES" : "NO") << '\n';  // not graphical
}
