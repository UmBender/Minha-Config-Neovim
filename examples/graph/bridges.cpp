// Problem: A road network; print the roads whose closure disconnects some cities (bridges), the
//   number of 2-edge-connected regions and the degree of each region in the bridge tree.
// Input:
//   7 8
//   0 1
//   1 2
//   2 0
//   2 3
//   3 4
//   4 5
//   5 3
//   5 6
// Output:
//   2-3 5-6
//   3
//   1 2 1
#include <bits/stdc++.h>
using namespace std;

#include "graph/bridges.cpp"  // in a solution: <leader>rl -> graph/bridges

int main() {
    int n, m;
    cin >> n >> m;
    vector<pair<int, int>> edges(m);
    for (auto &[u, v] : edges) cin >> u >> v;
    TwoEdgeCC t(n, edges);
    for (int e : t.bridges()) cout << edges[e].first << '-' << edges[e].second << ' ';
    cout << '\n' << t.count << '\n';
    auto tree = t.tree();
    for (int c = 0; c < t.count; c++) cout << tree[c].size() << " \n"[c == t.count - 1];
}
