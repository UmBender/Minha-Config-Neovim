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

#include "dsa/dsu-rollback.cpp"       // in a solution: <leader>rl -> dsa/dsu-rollback
#include "dsa/offline-deletions.cpp"  // and dsa/offline-deletions

int main() {
    int n, q;
    cin >> n >> q;
    OfflineDeletion<pair<int, int>> od;
    vector<pair<int, int>> ask;
    while (q--) {
        char op;
        int a, b;
        cin >> op >> a >> b;
        if (a > b) swap(a, b);  // undirected: one key per edge
        if (op == '+') od.insert({a, b});
        else if (op == '-') od.remove({a, b});
        else od.query(), ask.push_back({a, b});
    }

    RollbackDSU dsu(n);
    // presets: each one runs od, so use a copy for the second
    auto od2 = od;
    vector<int> comps = componentCounts(od, dsu);
    vector<int> conn = connectedAt(od2, dsu, ask);
    // general form, for any structure with insert + undo-last:
    //   od.run([&](auto e) { dsu.unite(e.first, e.second); }, [&] { dsu.undo(); }, [&](int qi) { ... });
    for (int i = 0; i < (int)ask.size(); i++) cout << comps[i] << ' ' << (conn[i] ? "YES" : "NO") << '\n';
}
