// Problem: the smallest circle covering every point: print its center and radius.
// Input:
//   5
//   0 0
//   4 0
//   0 3
//   1 1
//   2 1
// Output:
//   2.000 1.500 2.500
#include <bits/stdc++.h>
using namespace std;

#include "geometry/point.cpp"             // required by geometry/enclosing-circle (inserted with it)
#include "geometry/enclosing-circle.cpp"  // in a solution: <leader>rl -> geometry/enclosing-circle

using P = Point<long double>;

int main() {
    int n;
    cin >> n;
    vector<P> pts(n);
    for (auto &q : pts) cin >> q;
    auto [o, r] = enclosingCircle(pts);
    cout << fixed << setprecision(3) << o << ' ' << r << '\n';
}
