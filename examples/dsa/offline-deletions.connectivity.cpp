// Problem: dynamic connectivity. n nodes, q operations: "+ a b" adds an edge, "- a b" removes one
//   copy of it, "? a b" prints the number of components and whether a and b are connected.
// Input:
//   4 7
//   + 0 1
//   + 1 2
//   ? 0 2
//   - 0 1
//   ? 0 2
//   + 0 2
//   ? 0 1
// Output:
//   2 YES
//   3 NO
//   2 YES
#include <bits/stdc++.h>
using namespace std;

#include "dsa/offline-deletions.connectivity.cpp"  // in a solution: <leader>rl -> dsa/offline-deletions -> connectivity

int main() {
    int n, q;
    cin >> n >> q;
    DynamicConnectivity dc(n);
    while (q--) {
        char op;
        int a, b;
        cin >> op >> a >> b;
        if (op == '+') dc.addEdge(a, b);
        else if (op == '-') dc.removeEdge(a, b);
        else dc.query(a, b);
    }
    dc.run();
    for (int i = 0; i < (int)dc.comps.size(); i++)
        cout << dc.comps[i] << ' ' << (dc.connected[i] ? "YES" : "NO") << '\n';
}
