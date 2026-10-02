// Problem: for each pair of lines, each given by two of its points, print their common point, or
//   PARALLEL, or SAME.
// Input:
//   3
//   0 0 2 2 0 2 2 0
//   0 0 1 1 0 1 1 2
//   0 0 1 1 3 3 5 5
// Output:
//   1.000 1.000
//   PARALLEL
//   SAME
#include <bits/stdc++.h>
using namespace std;

#include "geometry/point.cpp"              // required by geometry/line-intersection (inserted with it)
#include "geometry/line-intersection.cpp"  // in a solution: <leader>rl -> geometry/line-intersection

using P = Point<long double>;

int main() {
    int q;
    cin >> q;
    cout << fixed << setprecision(3);
    while (q--) {
        P a, b, c, d;
        cin >> a >> b >> c >> d;
        auto [k, p] = lineInter(a, b, c, d);
        if (k == 1) cout << p << '\n';
        else cout << (k == 0 ? "PARALLEL" : "SAME") << '\n';
    }
}
