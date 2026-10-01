// Problem: n numbers and q queries: "1 i x" adds x to a[i], "2 l r" prints the sum of a[l..r),
//   "3 s" prints the shortest prefix length with sum >= s (all a[i] >= 0; n + 1 if none).
// Input:
//   5 5
//   1 2 3 4 5
//   2 0 5
//   1 1 10
//   2 0 2
//   3 13
//   3 100
// Output:
//   15
//   13
//   2
//   6
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

#include "dsa/fenwick-tree.cpp"  // in a solution: <leader>rl -> dsa/fenwick-tree -> normal

int main() {
    int n, q;
    cin >> n >> q;
    vector<ll> a(n);
    for (auto &x : a) cin >> x;
    Fenwick fw(a);  // T = long long deduced from a; Fenwick fw(n) starts with zeros
    while (q--) {
        int type;
        cin >> type;
        if (type == 1) {
            int i;
            ll x;
            cin >> i >> x;
            fw.add(i, x);
        } else if (type == 2) {
            int l, r;
            cin >> l >> r;
            cout << fw.sum(l, r) << '\n';
        } else {
            ll s;
            cin >> s;
            cout << fw.lowerBound(s) << '\n';
        }
    }
}
