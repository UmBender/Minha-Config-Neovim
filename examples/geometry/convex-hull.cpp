// Problem: the convex hull of a set of points: its vertices counterclockwise, then also with the
//   points lying on its edges.
// Input:
//   7
//   0 0
//   2 0
//   1 0
//   2 2
//   0 2
//   1 1
//   2 2
// Output:
//   4: (0 0) (2 0) (2 2) (0 2)
//   5: (0 0) (1 0) (2 0) (2 2) (0 2)
#include <bits/stdc++.h>
using namespace std;

#include "geometry/point.cpp"        // required by geometry/convex-hull (inserted with it)
#include "geometry/convex-hull.cpp"  // in a solution: <leader>rl -> geometry/convex-hull

using P = Point<long long>;

int main() {
    int n;
    cin >> n;
    vector<P> pts(n);
    for (auto &q : pts) cin >> q;
    for (bool collinear : {false, true}) {
        vector<int> h = convexHull(pts, collinear);  // indices into pts
        cout << h.size() << ':';
        for (int i : h) cout << " (" << pts[i] << ')';
        cout << '\n';
    }
}
