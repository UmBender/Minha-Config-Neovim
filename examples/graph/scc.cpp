// Problem: Count the strongly connected components of a directed graph and print the condensation
//   DAG in topological order.
// Input:
//   6 7
//   0 1
//   1 2
//   2 0
//   2 3
//   3 4
//   4 3
//   5 4
// Output:
//   3
//   component 0: 5 -> 2
//   component 1: 0 1 2 -> 2
//   component 2: 3 4 ->
#include <bits/stdc++.h>
using namespace std;

#include "graph/scc.cpp"  // in a solution: <leader>rl -> graph/scc

int main() {
    int n, m;
    cin >> n >> m;
    vector<vector<int>> g(n);
    for (int i = 0; i < m; i++) {
        int u, v;
        cin >> u >> v;
        g[u].push_back(v);
    }
    SCC s(g);
    cout << s.count << '\n';
    auto comps = s.components();
    auto dag = s.condensation(g);
    for (int c = 0; c < s.count; c++) {
        cout << "component " << c << ":";
        for (int v : comps[c]) cout << ' ' << v;
        cout << " ->";
        for (int d : dag[c]) cout << ' ' << d;
        cout << '\n';
    }
}
