// Problem: Order the vertices so that s comes first, t last, and every other vertex has a neighbor
//   before it and one after it. Print the order (vertices sorted by their number).
// Input:
//   5 6 0 4
//   0 1
//   1 2
//   2 4
//   0 3
//   3 2
//   1 3
// Output:
//   0 3 1 2 4
#include <bits/stdc++.h>
using namespace std;

#include "graph/st-numbering.cpp"  // in a solution: <leader>rl -> graph/st-numbering

int main() {
    int n, m, s, t;
    cin >> n >> m >> s >> t;
    vector<vector<int>> g(n);
    for (int i = 0; i < m; i++) {
        int u, v;
        cin >> u >> v;
        g[u].push_back(v), g[v].push_back(u);
    }
    auto num = stNumbering(g, s, t);
    if (num.empty()) {
        cout << "NO\n";
        return 0;
    }
    vector<int> order(n);
    for (int v = 0; v < n; v++) order[num[v]] = v;
    for (int v : order) cout << v << ' ';
    cout << '\n';
}
