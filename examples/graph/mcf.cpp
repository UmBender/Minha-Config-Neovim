// Problem: Ship goods from 0 to n-1 through directed routes with capacity and cost per unit. Print
//   the cheapest cost to ship k units, then the max amount and its min cost.
// Input:
//   4 5 3
//   0 1 2 1
//   0 2 2 4
//   1 2 1 1
//   1 3 1 5
//   2 3 3 1
// Output:
//   3 13
//   4 19
#include <bits/stdc++.h>
using namespace std;

#include "graph/mcf.cpp"  // in a solution: <leader>rl -> graph/mcf

int main() {
    int n, m, k;
    cin >> n >> m >> k;
    vector<array<long long, 4>> edges(m);
    for (auto &[u, v, cap, cost] : edges) cin >> u >> v >> cap >> cost;
    MinCostFlow limited(n), full(n);
    for (auto [u, v, cap, cost] : edges) limited.addEdge(u, v, cap, cost), full.addEdge(u, v, cap, cost);
    auto [f1, c1] = limited.flow(0, n - 1, k);
    cout << f1 << ' ' << c1 << '\n';
    auto [f2, c2] = full.flow(0, n - 1);
    cout << f2 << ' ' << c2 << '\n';
}
