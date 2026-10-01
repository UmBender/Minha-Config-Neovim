// Problem: n numbers and q queries: "1 i x" sets a[i] = x, "2 l r" prints the minimum of a[l..r)
//   and "3 l x" prints the first index r >= l where the prefix sum from l exceeds x (n if none).
// Input:
//   5 4
//   5 3 8 1 4
//   2 0 5
//   1 3 9
//   2 1 4
//   3 0 15
// Output:
//   1
//   3
//   2
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

#include "dsa/segtree.cpp"  // in a solution: <leader>rl -> dsa/segtree

int main() {
    int n, q;
    cin >> n >> q;
    vector<ll> a(n);
    for (auto &x : a) cin >> x;

    auto mn = minSegtree(a);   // preset: min over ranges
    auto sum = sumSegtree(a);  // preset: sums, used for the binary search
    // the general form, e.g. for gcd: Segtree g(a, 0LL, [](ll x, ll y) { return gcd(x, y); });

    while (q--) {
        int type;
        cin >> type;
        if (type == 1) {
            int i;
            ll x;
            cin >> i >> x;
            mn.set(i, x), sum.set(i, x);
        } else if (type == 2) {
            int l, r;
            cin >> l >> r;
            cout << mn.query(l, r) << '\n';
        } else {
            int l;
            ll x;
            cin >> l >> x;
            // largest r with sum(l, r) <= x  ->  the next index is the first one that exceeds x
            cout << sum.maxRight(l, [&](ll s) { return s <= x; }) << '\n';
        }
    }
}
