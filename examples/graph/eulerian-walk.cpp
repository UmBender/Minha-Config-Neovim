// Problem: Draw a figure without lifting the pen: print the vertex sequence and edge order of an
//   Eulerian path (undirected), then check whether the directed version has an Eulerian cycle.
// Input:
//   5 6
//   0 1
//   1 2
//   2 0
//   2 3
//   3 4
//   4 2
// Output:
//   0 1 2 3 4 2 0
//   0 1 3 4 5 2
//   directed cycle
#include <bits/stdc++.h>
using namespace std;

#include "graph/eulerian-walk.cpp"  // in a solution: <leader>rl -> graph/eulerian-walk

int main() {
    int n, m;
    cin >> n >> m;
    vector<pair<int, int>> edges(m);
    for (auto &[u, v] : edges) cin >> u >> v;
    auto res = eulerianPath(n, edges, false, false);
    if (res) {
        auto &[verts, ids] = *res;
        for (int v : verts) cout << v << ' ';
        cout << '\n';
        for (int e : ids) cout << e << ' ';
        cout << '\n';
    }
    cout << (eulerianPath(n, edges, true, true) ? "directed cycle" : "no directed cycle") << '\n';
}
