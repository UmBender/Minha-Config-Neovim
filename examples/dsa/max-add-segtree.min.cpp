// Problem: n rooms with a[i] people; q events "1 l r v" (v people enter each room of [l, r),
//   v may be negative) or "2 l r": print the fewest people in a room of [l, r).
// Input:
//   5 4
//   3 1 4 1 5
//   2 0 5
//   1 1 4 3
//   2 0 3
//   2 3 5
// Output:
//   1
//   3
//   4
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

#include "dsa/max-add-segtree.min.cpp"  // in a solution: <leader>rl -> dsa/max-add-segtree -> min

int main() {
    int n, q;
    cin >> n >> q;
    vector<ll> a(n);
    for (auto &x : a) cin >> x;

    MinAddSegtree mn(a);  // T = long long deduced from a

    while (q--) {
        int type, l, r;
        cin >> type >> l >> r;
        if (type == 1) {
            ll v;
            cin >> v;
            mn.add(l, r, v);
        } else {
            cout << mn.query(l, r) << '\n';
        }
    }
}
