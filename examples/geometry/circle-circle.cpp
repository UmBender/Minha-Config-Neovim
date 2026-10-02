// Problem: pairs of circles (x y r each): print their common points.
// Input:
//   3
//   0 0 5 8 0 5
//   0 0 2 5 0 3
//   0 0 1 5 0 1
// Output:
//   2 (4.000 -3.000) (4.000 3.000)
//   1 (2.000 0.000)
//   0
#include <bits/stdc++.h>
using namespace std;

#include "geometry/point.cpp"          // required by geometry/circle-circle (inserted with it)
#include "geometry/circle-circle.cpp"  // in a solution: <leader>rl -> geometry/circle-circle

using P = Point<long double>;

int main() {
    int q;
    cin >> q;
    cout << fixed << setprecision(3);
    while (q--) {
        P c1, c2;
        long double r1, r2;
        cin >> c1 >> r1 >> c2 >> r2;
        auto v = circleCircle(c1, r1, c2, r2);
        cout << v.size();
        for (P p : v) cout << " (" << p << ')';
        cout << '\n';
    }
}
