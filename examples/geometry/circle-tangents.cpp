// Problem: a circle (x y r) and a point outside it: print where the tangents from the point touch
//   the circle; then the number of common tangents of the circle and another one.
// Input:
//   0 0 5
//   0 10
//   12 0 3
// Output:
//   -4.330 2.500
//   4.330 2.500
//   4
#include <bits/stdc++.h>
using namespace std;

#include "geometry/point.cpp"            // required by geometry/circle-tangents (inserted with it)
#include "geometry/circle-tangents.cpp"  // in a solution: <leader>rl -> geometry/circle-tangents

using P = Point<long double>;

int main() {
    P c, q, c2;
    long double r, r2;
    cin >> c >> r >> q >> c2 >> r2;
    cout << fixed << setprecision(3);
    for (auto [t, _] : circleTangents(c, r, q, 0.0L)) cout << t << '\n';  // a point: radius 0
    cout << circleTangents(c, r, c2, r2).size() << '\n';
}
