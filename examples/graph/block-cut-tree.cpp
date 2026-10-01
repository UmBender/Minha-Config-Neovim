// Problem: Print the articulation points of an undirected graph and its biconnected components.
// Input:
//   6 7
//   0 1
//   1 2
//   2 0
//   1 3
//   3 4
//   4 1
//   4 5
// Output:
//   1 4
//   4 5
//   1 3 4
//   0 1 2
#include <bits/stdc++.h>
using namespace std;

#include "graph/block-cut-tree.cpp"  // in a solution: <leader>rl -> graph/block-cut-tree

int main() {
    int n, m;
    cin >> n >> m;
    vector<vector<int>> g(n);
    for (int i = 0; i < m; i++) {
        int u, v;
        cin >> u >> v;
        g[u].push_back(v), g[v].push_back(u);
    }
    BlockCutTree b(g);
    for (int v = 0; v < n; v++)
        if (b.isCut[v]) cout << v << ' ';
    cout << '\n';
    for (auto blk : b.blocks) {
        sort(blk.begin(), blk.end());
        for (int v : blk) cout << v << ' ';
        cout << '\n';
    }
}
