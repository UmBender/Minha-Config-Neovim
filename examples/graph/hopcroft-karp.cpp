// Problem: Workers (left) and jobs (right), each worker can do some jobs. Print the max number of
//   assignments, who does what, and a minimum set of workers/jobs touching every pair.
// Input:
//   3 3 5
//   0 0
//   0 1
//   1 0
//   2 0
//   2 2
// Output:
//   3
//   1 0 2
//   workers: 0 1 2 jobs:
#include <bits/stdc++.h>
using namespace std;

#include "graph/hopcroft-karp.cpp"  // in a solution: <leader>rl -> graph/hopcroft-karp

int main() {
    int nl, nr, m;
    cin >> nl >> nr >> m;
    HopcroftKarp hk(nl, nr);
    for (int i = 0; i < m; i++) {
        int u, v;
        cin >> u >> v;
        hk.addEdge(u, v);
    }
    cout << hk.maxMatching() << '\n';
    for (int u = 0; u < nl; u++) cout << hk.matchL[u] << " \n"[u == nl - 1];
    auto [cl, cr] = hk.vertexCover();
    cout << "workers:";
    for (int u : cl) cout << ' ' << u;
    cout << " jobs:";
    for (int v : cr) cout << ' ' << v;
    cout << '\n';
}
