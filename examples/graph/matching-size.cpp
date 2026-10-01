// Problem: Only the size of a maximum matching is needed (no pairs). Two triangles joined by an
//   edge, plus an isolated vertex.
// Input:
//   7 7
//   0 1
//   1 2
//   2 0
//   3 4
//   4 5
//   5 3
//   2 3
// Output:
//   3
#include <bits/stdc++.h>
using namespace std;

#include "graph/matching-size.cpp"  // in a solution: <leader>rl -> graph/matching-size

int main() {
    int n, m;
    cin >> n >> m;
    vector<pair<int, int>> edges(m);
    for (auto &[u, v] : edges) cin >> u >> v;
    cout << matchingSize(n, edges) << '\n';
}
