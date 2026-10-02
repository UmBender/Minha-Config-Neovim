// Problem: a simple polygon and query points: print INSIDE, OUTSIDE or BOUNDARY for each; then the
//   same for the triangle of its first three vertices (convex: O(log n) per query).
// Input:
//   4
//   1 1
//   5 1
//   5 5
//   3 2
//   5
//   4 2
//   3 3
//   5 3
//   3 2
//   0 0
// Output:
//   INSIDE INSIDE
//   OUTSIDE BOUNDARY
//   BOUNDARY BOUNDARY
//   BOUNDARY INSIDE
//   OUTSIDE OUTSIDE
#include <bits/stdc++.h>
using namespace std;

#include "geometry/point.cpp"             // required by geometry/point-in-polygon (inserted with it)
#include "geometry/point-in-polygon.cpp"  // in a solution: <leader>rl -> geometry/point-in-polygon

using P = Point<long long>;

int main() {
    int n, m;
    cin >> n;
    vector<P> p(n);
    for (auto &q : p) cin >> q;
    vector<P> tri{p[0], p[1], p[2]};  // convex, counterclockwise
    const char *name[] = {"OUTSIDE", "BOUNDARY", "INSIDE"};
    cin >> m;
    while (m--) {
        P q;
        cin >> q;
        cout << name[inPolygon(p, q) + 1] << ' ' << name[inConvex(tri, q) + 1] << '\n';
    }
}
