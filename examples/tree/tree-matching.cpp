// Problem: the largest set of tree edges with no common endpoint: its size and its edges.
// Input:
//   7
//   0 1
//   0 2
//   0 3
//   1 5
//   1 5
//   3 6
// Output:
//   3
//   0 2
//   1 5
//   3 6
#include <bits/stdc++.h>
using namespace std;

#include "tree/tree-matching.cpp"  // in a solution: <leader>rl -> tree/tree-matching

int main() {
    int n;
    cin >> n;
    vector<vector<int>> g(n);
    for (int i = 0; i + 1 < n; i++) {
        int a, b;
        cin >> a >> b;
        g[a].push_back(b), g[b].push_back(a);
    }
    auto mate = treeMatching(g);
    cout << (n - count(mate.begin(), mate.end(), -1)) / 2 << '\n';
    for (int v = 0; v < n; v++)
        if (v < mate[v]) cout << v << ' ' << mate[v] << '\n';
}
