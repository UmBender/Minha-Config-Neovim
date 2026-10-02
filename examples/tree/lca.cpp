// Problem: a rooted tree given by parents (vertex 0 is the root); answer queries "u v" with the LCA
//   of u and v and the distance between them.
// Input:
//   8 3
//   0 0 0 1 1 3 6
//   4 5
//   5 7
//   6 3
// Output:
//   1 2
//   0 5
//   3 1
#include <bits/stdc++.h>
using namespace std;

#include "tree/lca.cpp"  // in a solution: <leader>rl -> tree/lca

int main() {
    int n, q;
    cin >> n >> q;
    vector<vector<int>> g(n);
    for (int v = 1; v < n; v++) {
        int p;
        cin >> p;
        g[p].push_back(v), g[v].push_back(p);
    }
    LCA l(g);
    while (q--) {
        int u, v;
        cin >> u >> v;
        cout << l.lca(u, v) << ' ' << l.dist(u, v) << '\n';
    }
}
