// Problem: Each of n rooms has a portal to f[i]. For each query "v k" print the room reached after
//   taking k portals from v (k up to 1e18), and the length of the cycle v ends up in.
// Input:
//   6
//   1 2 0 2 3 4
//   3
//   5 3
//   5 1000000000000000000
//   0 4
// Output:
//   2 3
//   0 3
//   1 3
#include <bits/stdc++.h>
using namespace std;

#include "graph/functional-forest.cpp"  // in a solution: <leader>rl -> graph/functional-forest

int main() {
    int n, q;
    cin >> n;
    vector<int> f(n);
    for (auto &x : f) cin >> x;
    FunctionalGraph fg(f);
    cin >> q;
    while (q--) {
        int v;
        long long k;
        cin >> v >> k;
        cout << fg.jump(v, k) << ' ' << fg.cycles[fg.cycleId[v]].size() << '\n';
    }
}
