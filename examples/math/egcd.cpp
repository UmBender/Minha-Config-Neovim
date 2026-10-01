// Problem: for each query a b c (a, b >= 1) print integers x y with a*x + b*y = c and the smallest
//   x >= 0, or -1 if there is none; then the inverse of a modulo b (-1 if it doesn't exist).
// Input:
//   3
//   3 5 7
//   4 6 7
//   12 18 30
// Output:
//   4 -1 2
//   -1 -1
//   1 1 -1
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

#include "math/egcd.cpp"  // in a solution: <leader>rl -> math/egcd

int main() {
    int q;
    cin >> q;
    while (q--) {
        ll a, b, c, x, y;
        cin >> a >> b >> c;
        ll g = egcd(a, b, x, y);
        if (c % g) {
            cout << -1;
        } else {
            ll step = b / g;  // solutions: x + t * b/g, y - t * a/g
            x = ((__int128)x * (c / g) % step + step) % step;
            y = (c - a * x) / b;
            cout << x << ' ' << y;
        }
        cout << ' ' << invMod(a, b) << '\n';
    }
}
