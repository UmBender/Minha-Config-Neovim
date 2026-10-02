// Problem: the longest path of a weighted tree: its length and its vertices.
// Input:
//   5
//   0 1 3
//   1 2 4
//   1 3 1
//   3 4 5
// Output:
//   10
//   2 1 3 4
#include <bits/stdc++.h>
using namespace std;

#include "tree/tree-diameter.cpp"  // in a solution: <leader>rl -> tree/tree-diameter

int main() {
    int n;
    cin >> n;
    vector<vector<pair<int, long long>>> g(n);
    for (int i = 0; i + 1 < n; i++) {
        int a, b;
        long long w;
        cin >> a >> b >> w;
        g[a].push_back({b, w}), g[b].push_back({a, w});
    }
    auto [len, path] = treeDiameter(g);
    cout << len << '\n';
    for (int i = 0; i < (int)path.size(); i++) cout << path[i] << " \n"[i + 1 == (int)path.size()];
}
