// Problem: Max flow from 0 to n-1 in a directed network; print the flow, the flow on each pipe and
//   the vertices on the source side of a minimum cut.
// Input:
//   4 5
//   0 1 3
//   0 2 2
//   1 2 5
//   1 3 2
//   2 3 3
// Output:
//   5
//   3 2 1 2 3
//   0
#include <bits/stdc++.h>
using namespace std;

#include "graph/dinic.cpp"  // in a solution: <leader>rl -> graph/dinic

int main() {
    int n, m;
    cin >> n >> m;
    Dinic d(n);  // long long capacities
    for (int i = 0; i < m; i++) {
        int u, v;
        long long c;
        cin >> u >> v >> c;
        d.addEdge(u, v, c);
    }
    cout << d.flow(0, n - 1) << '\n';
    for (int i = 0; i < m; i++) cout << d.flowOn(i) << " \n"[i == m - 1];
    auto side = d.minCut();
    for (int v = 0; v < n; v++)
        if (side[v]) cout << v << ' ';
    cout << '\n';
}
