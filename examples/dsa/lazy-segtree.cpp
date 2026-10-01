// Problem: "1 l r x" adds x to a[l..r), "2 l r" prints the sum of a[l..r) (preset), then
//   "3 l r b c" maps a[i] -> b * a[i] + c (mod 998244353) and "4 l r" prints the sum mod p
//   (general LazySegtree with custom lambdas).
// Input:
//   4
//   1 2 3 4
//   5
//   1 0 2 10
//   2 0 4
//   3 1 3 2 1
//   4 0 4
//   2 1 2
// Output:
//   30
//   47
//   12
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

#include "dsa/lazy-segtree.cpp"  // in a solution: <leader>rl -> dsa/lazy-segtree

const ll MOD = 998244353;
struct Node { ll sum, len; };  // store the length when the update depends on it
struct Aff { ll b, c; };       // x -> b x + c

int main() {
    int n, q;
    cin >> n;
    vector<ll> a(n);
    for (auto &x : a) cin >> x;
    cin >> q;

    RangeAddSum<ll> seg(a);  // preset: range add + range sum

    vector<Node> init;
    for (ll x : a) init.push_back({x % MOD, 1});
    LazySegtree aff(
        init, Node{0, 0}, Aff{1, 0},
        [](Node x, Node y) { return Node{(x.sum + y.sum) % MOD, x.len + y.len}; },
        [](Aff f, Node x) { return Node{(f.b * x.sum + f.c * x.len) % MOD, x.len}; },
        [](Aff f, Aff g) { return Aff{f.b * g.b % MOD, (f.b * g.c + f.c) % MOD}; });  // f after g

    while (q--) {
        int type, l, r;
        cin >> type >> l >> r;
        if (type == 1) {
            ll x;
            cin >> x;
            seg.add(l, r, x);
            aff.apply(l, r, Aff{1, x});
        } else if (type == 2) {
            cout << seg.sum(l, r) << '\n';
        } else if (type == 3) {
            ll b, c;
            cin >> b >> c;
            aff.apply(l, r, Aff{b, c});
        } else {
            cout << aff.query(l, r).sum << '\n';
        }
    }
}
