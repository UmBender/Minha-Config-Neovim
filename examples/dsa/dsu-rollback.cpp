// Problem: n nodes and a list of edges. For each "try k a1 b1 ... ak bk" print whether adding those
//   k edges (temporarily) connects node 0 to node n - 1; "add a b" adds an edge for good.
// Input:
//   4 4
//   add 0 1
//   try 1 2 3
//   try 2 1 2 2 3
//   try 1 1 3
// Output:
//   NO
//   YES
//   YES
#include <bits/stdc++.h>
using namespace std;

#include "dsa/dsu-rollback.cpp"  // in a solution: <leader>rl -> dsa/dsu-rollback

int main() {
    int n, q;
    cin >> n >> q;
    RollbackDSU dsu(n);
    while (q--) {
        string op;
        cin >> op;
        if (op == "add") {
            int a, b;
            cin >> a >> b;
            dsu.unite(a, b);
        } else {
            int k;
            cin >> k;
            int t = dsu.time();  // snapshot
            for (int i = 0; i < k; i++) {
                int a, b;
                cin >> a >> b;
                dsu.unite(a, b);
            }
            cout << (dsu.same(0, n - 1) ? "YES" : "NO") << '\n';
            dsu.rollback(t);  // forget the temporary edges
        }
    }
}
