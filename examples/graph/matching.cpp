// Problem: Pair up students; "u v" means u and v can work together. Print the max number of pairs
//   and the pairs (an odd cycle 0-1-2 plus a tail, which needs a blossom).
// Input:
//   6 6
//   0 1
//   1 2
//   2 0
//   2 3
//   3 4
//   4 5
// Output:
//   3
//   0 1
//   2 3
//   4 5
#include <bits/stdc++.h>
using namespace std;

#include "graph/matching.cpp"  // in a solution: <leader>rl -> graph/matching

int main() {
    int n, m;
    cin >> n >> m;
    vector<vector<int>> g(n);
    for (int i = 0; i < m; i++) {
        int u, v;
        cin >> u >> v;
        g[u].push_back(v), g[v].push_back(u);
    }
    auto mate = generalMatching(g);
    cout << (n - count(mate.begin(), mate.end(), -1)) / 2 << '\n';
    for (int v = 0; v < n; v++)
        if (mate[v] > v) cout << v << ' ' << mate[v] << '\n';
}
