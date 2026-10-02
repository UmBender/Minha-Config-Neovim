// Problem: a convex polygon and directed lines: for each line, the area of the part of the polygon
//   on its left.
// Input:
//   4
//   0 0
//   4 0
//   4 4
//   0 4
//   3
//   1 0 1 1
//   0 0 4 4
//   0 5 1 5
// Output:
//   4.00
//   8.00
//   0.00
#include <bits/stdc++.h>
using namespace std;

#include "geometry/point.cpp"         // required by geometry/polygon-cut (inserted with it)
#include "geometry/polygon-cut.cpp"   // in a solution: <leader>rl -> geometry/polygon-cut
#include "geometry/polygon-area.cpp"  // for the areas

using P = Point<long double>;

int main() {
    int n, q;
    cin >> n;
    vector<P> p(n);
    for (auto &v : p) cin >> v;
    cin >> q;
    cout << fixed << setprecision(2);
    while (q--) {
        P a, b;
        cin >> a >> b;
        cout << fabsl(polyArea2(polygonCut(p, a, b))) / 2 << '\n';
    }
}
