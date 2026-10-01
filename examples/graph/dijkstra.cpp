// Problem: Roads between cities with lengths; some cities have a hospital. Print the distance from
//   every city to its nearest hospital (-1 if none), then the route from city 4 to its hospital.
// Input:
//   5 5 2
//   0 1 4
//   1 2 1
//   2 3 7
//   0 2 9
//   3 4 2
//   0 3
// Output:
//   0 4 5 0 2
//   4 3
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

#include "graph/dijkstra.cpp"  // in a solution: <leader>rl -> graph/dijkstra

int main() {
    int n, m, h;
    cin >> n >> m >> h;
    vector<vector<pair<int, ll>>> g(n);
    for (int i = 0; i < m; i++) {
        int u, v;
        ll w;
        cin >> u >> v >> w;
        g[u].push_back({v, w}), g[v].push_back({u, w});
    }
    vector<int> hospitals(h);
    for (auto &x : hospitals) cin >> x;
    Dijkstra d(g, hospitals);  // multi-source
    for (int v = 0; v < n; v++) cout << (d.reached(v) ? d.dist[v] : -1) << " \n"[v == n - 1];
    auto p = d.path(4);
    reverse(p.begin(), p.end());  // path goes hospital -> 4
    for (int v : p) cout << v << ' ';
    cout << '\n';
}
