// Problem: n people and m friendships "a b" given one by one. After each one print the number of
//   friend groups and the size of a's group; at the end print every group.
// Input:
//   5 4
//   0 1
//   2 3
//   1 0
//   1 3
// Output:
//   4 2
//   3 2
//   3 2
//   2 4
//   0 1 2 3
//   4
#include <bits/stdc++.h>
using namespace std;

#include "dsa/dsu.cpp"  // in a solution: <leader>rl -> dsa/dsu -> normal

int main() {
    int n, m;
    cin >> n >> m;
    DSU dsu(n);
    while (m--) {
        int a, b;
        cin >> a >> b;
        dsu.unite(a, b);  // false if they were already together
        cout << dsu.count() << ' ' << dsu.size(a) << '\n';
    }
    for (auto &g : dsu.groups()) {
        for (int i = 0; i < (int)g.size(); i++) cout << g[i] << " \n"[i + 1 == (int)g.size()];
    }
}
