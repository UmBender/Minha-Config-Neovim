// Problem: a circle (center, radius) and lines through two points: print where each line meets it.
// Input:
//   0 0 5
//   3
//   -10 3 10 3
//   7 5 8 5
//   0 6 1 6
// Output:
//   2 (-4.000 3.000) (4.000 3.000)
//   1 (0.000 5.000)
//   0
#include <bits/stdc++.h>
using namespace std;

#include "geometry/point.cpp"        // required by geometry/circle-line (inserted with it)
#include "geometry/circle-line.cpp"  // in a solution: <leader>rl -> geometry/circle-line

using P = Point<long double>;

int main() {
    P c, a, b;
    long double r;
    int q;
    cin >> c >> r >> q;
    cout << fixed << setprecision(3);
    while (q--) {
        cin >> a >> b;
        auto v = circleLine(c, r, a, b);
        cout << v.size();
        for (P p : v) cout << " (" << p << ')';
        cout << '\n';
    }
}
