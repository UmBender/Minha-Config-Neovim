// Problem: static array a of n numbers and q queries on a[l, r). "1 l r k" prints the k-th smallest
//   (0-indexed), "2 l r x y" the count of values in [x, y), "3 l r x" the largest value < x
//   ("none" if there is none) and "4 l r x" the sum of values < x.
// Input:
//   8
//   5 2 7 2 9 5 1 7
//   6
//   1 0 8 3
//   1 2 6 0
//   2 1 7 2 6
//   3 0 4 5
//   4 0 8 6
//   4 1 4 8
// Output:
//   5
//   2
//   3
//   2
//   15
//   11
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

#include "dsa/wavelet-matrix.cpp"  // in a solution: <leader>rl -> dsa/wavelet-matrix -> normal

int main() {
    int n, q;
    cin >> n;
    vector<ll> a(n);
    for (auto &x : a) cin >> x;
    cin >> q;

    WaveletMatrix wm(a);  // T deduced from a

    while (q--) {
        int type, l, r;
        cin >> type >> l >> r;
        if (type == 1) {
            int k;
            cin >> k;
            cout << wm.kth(l, r, k) << '\n';
        } else if (type == 2) {
            ll x, y;
            cin >> x >> y;
            cout << wm.count(l, r, x, y) << '\n';
        } else if (type == 3) {
            ll x;
            cin >> x;
            auto p = wm.prev(l, r, x);
            if (p) cout << *p << '\n';
            else cout << "none\n";
        } else {
            ll x;
            cin >> x;
            cout << wm.sum(l, r, LLONG_MIN, x) << '\n';
        }
    }
}
