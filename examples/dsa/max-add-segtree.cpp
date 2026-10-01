// Problem: n rooms with a[i] people; q events "1 l r v" (v people enter each room of [l, r),
//   v may be negative) or "2 l r": print the most and the fewest people in a room of [l, r).
// Input:
//   5 4
//   3 1 4 1 5
//   2 0 5
//   1 1 4 3
//   2 0 3
//   2 3 5
// Output:
//   5 1
//   7 3
//   5 4
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

#include "dsa/max-add-segtree.cpp"  // in a solution: <leader>rl -> dsa/max-add-segtree

int main() {
    int n, q;
    cin >> n >> q;
    vector<ll> a(n);
    for (auto &x : a) cin >> x;

    MaxAddSegtree mx(a);  // T = long long deduced from a
    MinAddSegtree mn(a);  // preset: same API, minimum instead

    while (q--) {
        int type, l, r;
        cin >> type >> l >> r;
        if (type == 1) {
            ll v;
            cin >> v;
            mx.add(l, r, v), mn.add(l, r, v);
        } else {
            cout << mx.query(l, r) << ' ' << mn.query(l, r) << '\n';
        }
    }
}
