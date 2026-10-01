// Problem: n guests, each sits at table A (true) or B (false). Constraints: "1 a b" means a and b
//   sit together, "2 a b" means apart, "3 a" means a must sit at A. Also at most one of guests
//   0, 1, 2 sits at A. Print the assignment or IMPOSSIBLE.
// Input:
//   4 3
//   1 0 3
//   1 3 1
//   3 2
// Output:
//   BBAB
#include <bits/stdc++.h>
using namespace std;

#include "graph/scc.cpp"    // required by graph/2-sat (the picker inserts it)
#include "graph/2-sat.cpp"  // in a solution: <leader>rl -> graph/2-sat

int main() {
    int n, k;
    cin >> n >> k;
    TwoSat ts(n);
    while (k--) {
        int type, a, b;
        cin >> type >> a;
        if (type == 3) {
            ts.mustBe(a, true);
            continue;
        }
        cin >> b;
        if (type == 1) ts.equal(a, b);
        else ts.different(a, b);
    }
    ts.atMostOne({{0, true}, {1, true}, {2, true}});
    if (!ts.solve()) {
        cout << "IMPOSSIBLE\n";
        return 0;
    }
    for (int i = 0; i < n; i++) cout << (ts.value[i] ? 'A' : 'B');
    cout << '\n';
}
