// Problem: a sequence and q operations: "1 p x" inserts x before position p, "2 p" erases position p,
//   "3 l r" reverses a[l..r), "4 l r x" adds x to a[l..r), "5 l r" prints the sum and the minimum of
//   a[l..r). Print the final sequence.
// Input:
//   5
//   5 1 4 2 3
//   6
//   5 0 5
//   3 1 4
//   4 0 2 10
//   1 5 -2
//   5 1 6
//   2 0
// Output:
//   15 1
//   18 -2
//   12 4 1 3 -2
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

#include "dsa/treap.cpp"  // in a solution: <leader>rl -> dsa/treap

int main() {
    int n, q;
    cin >> n;
    ImplicitTreap t;  // values are long long (default T)
    for (int i = 0; i < n; i++) {
        ll x;
        cin >> x;
        t.insert(t.size(), x);  // push_back (or build at once: ImplicitTreap t(a))
    }
    cin >> q;
    while (q--) {
        int type;
        cin >> type;
        if (type == 1) {
            int p;
            ll x;
            cin >> p >> x;
            t.insert(p, x);
        } else if (type == 2) {
            int p;
            cin >> p;
            t.erase(p);
        } else if (type == 3) {
            int l, r;
            cin >> l >> r;
            t.reverse(l, r);
        } else if (type == 4) {
            int l, r;
            ll x;
            cin >> l >> r >> x;
            t.add(l, r, x);
        } else {
            int l, r;
            cin >> l >> r;
            cout << t.sum(l, r) << ' ' << t.min(l, r) << '\n';
        }
    }
    vector<ll> a = t.toVector();
    for (size_t i = 0; i < a.size(); i++) cout << a[i] << " \n"[i + 1 == a.size()];
}
