// Problem: Broadcast from station 0: each one-way link "u v w" costs w. Pick links so every
//   station is reached from 0 at minimum total cost; print the cost and the sender of each station.
// Input:
//   4 6
//   0 1 5
//   0 2 1
//   2 1 1
//   1 3 1
//   3 2 1
//   0 3 10
// Output:
//   3
//   2 0 1
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

#include "graph/directed-mst.cpp"  // in a solution: <leader>rl -> graph/directed-mst

int main() {
    int n, m;
    cin >> n >> m;
    vector<tuple<int, int, ll>> edges(m);
    for (auto &[u, v, w] : edges) cin >> u >> v >> w;
    auto res = directedMST(n, 0, edges);
    if (!res) {
        cout << "IMPOSSIBLE\n";
        return 0;
    }
    auto [cost, par] = *res;
    cout << cost << '\n';
    for (int v = 1; v < n; v++) cout << get<0>(edges[par[v]]) << " \n"[v == n - 1];
}
