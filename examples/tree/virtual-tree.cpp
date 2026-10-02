// Problem: a weighted tree (unit edges, rooted at 0) and queries with k marked vertices each; print
//   the total length of the smallest subtree connecting the marked vertices.
// Input:
//   9
//   0 1
//   0 2
//   1 3
//   1 4
//   4 5
//   2 6
//   6 7
//   6 8
//   3
//   2 3 5
//   3 5 7 8
//   1 6
// Output:
//   3
//   7
//   0
#include <bits/stdc++.h>
using namespace std;

#include "tree/lca.cpp"           // required by tree/virtual-tree (the picker inserts it)
#include "tree/virtual-tree.cpp"  // in a solution: <leader>rl -> tree/virtual-tree

int main() {
    int n;
    cin >> n;
    vector<vector<int>> g(n);
    for (int i = 0; i + 1 < n; i++) {
        int a, b;
        cin >> a >> b;
        g[a].push_back(b), g[b].push_back(a);
    }
    LCA l(g);
    int q;
    cin >> q;
    while (q--) {
        int k;
        cin >> k;
        vector<int> marked(k);
        for (int &v : marked) cin >> v;
        // the virtual tree's edges are compressed paths: their lengths add up to the answer
        auto [vs, edges] = virtualTree(l, marked);
        long long total = 0;
        for (auto [p, c] : edges) total += l.depth[c] - l.depth[p];
        cout << total << '\n';
    }
}
