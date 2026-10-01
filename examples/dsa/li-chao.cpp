// Problem: process q operations on lines y = a x + b, with |x| <= 1e9. "1 a b" adds a line,
//   "2 a b l r" adds it only for x in [l, r), "3 x" prints the minimum and the maximum at x
//   ("none" if no line covers x).
// Input:
//   8
//   3 0
//   1 2 3
//   1 -1 10
//   3 0
//   3 5
//   2 0 -100 10 20
//   3 15
//   3 25
// Output:
//   none
//   3 10
//   5 13
//   -100 33
//   -15 53
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

#include "dsa/li-chao.cpp"  // in a solution: <leader>rl -> dsa/li-chao

int main() {
    MinLiChao mn;  // preset: long long, x in [-1e9, 1e9]
    MaxLiChao mx;  // other ranges/types: LiChao<double, true> lc(lo, hi)
    int q;
    cin >> q;
    while (q--) {
        int type;
        cin >> type;
        if (type == 1) {
            ll a, b;
            cin >> a >> b;
            mn.addLine(a, b), mx.addLine(a, b);
        } else if (type == 2) {
            ll a, b, l, r;
            cin >> a >> b >> l >> r;
            mn.addSegment(a, b, l, r), mx.addSegment(a, b, l, r);
        } else {
            ll x;
            cin >> x;
            if (mn.query(x) == mn.NONE) cout << "none\n";
            else cout << mn.query(x) << ' ' << mx.query(x) << '\n';
        }
    }
}
