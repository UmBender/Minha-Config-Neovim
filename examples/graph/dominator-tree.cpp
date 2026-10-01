// Problem: Control flow graph with entry 0. For every block print its immediate dominator (-1 if
//   unreachable), then the blocks every path from 0 to 5 must pass through.
// Input:
//   7 8
//   0 1
//   1 2
//   1 3
//   2 4
//   3 4
//   4 5
//   5 1
//   6 5
// Output:
//   0 0 1 1 1 4 -1
//   4 1 0
#include <bits/stdc++.h>
using namespace std;

#include "graph/dominator-tree.cpp"  // in a solution: <leader>rl -> graph/dominator-tree

int main() {
    int n, m;
    cin >> n >> m;
    vector<vector<int>> g(n);
    for (int i = 0; i < m; i++) {
        int u, v;
        cin >> u >> v;
        g[u].push_back(v);
    }
    auto idom = dominatorTree(g, 0);
    for (int v = 0; v < n; v++) cout << idom[v] << " \n"[v == n - 1];
    for (int v = 5; v != 0; v = idom[v]) cout << idom[v] << ' ';
    cout << '\n';
}
