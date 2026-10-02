// Problem: two convex polygons (counterclockwise): print their Minkowski sum, and whether they
//   intersect (they do iff the origin is in A + (-B)).
// Input:
//   4
//   0 0
//   2 0
//   2 2
//   0 2
//   3
//   3 1
//   5 1
//   3 3
// Output:
//   (3 1) (7 1) (7 3) (5 5) (3 5)
//   APART
#include <bits/stdc++.h>
using namespace std;

#include "geometry/point.cpp"             // required by geometry/minkowski (inserted with it)
#include "geometry/minkowski.cpp"         // in a solution: <leader>rl -> geometry/minkowski
#include "geometry/point-in-polygon.cpp"  // for the intersection test

using P = Point<long long>;

int main() {
    int n, m;
    cin >> n;
    vector<P> a(n);
    for (auto &q : a) cin >> q;
    cin >> m;
    vector<P> b(m);
    for (auto &q : b) cin >> q;
    for (P q : minkowski(a, b)) cout << '(' << q << ") ";
    cout << '\n';
    vector<P> nb;  // -B is B rotated by 180 degrees: still convex and counterclockwise
    for (P q : b) nb.push_back(-q);
    cout << (inConvex(minkowski(a, nb), P{0, 0}) >= 0 ? "INTERSECT" : "APART") << '\n';
}
