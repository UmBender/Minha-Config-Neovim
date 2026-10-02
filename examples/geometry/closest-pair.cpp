// Problem: the squared distance between the two closest points, and which points they are.
// Input:
//   5
//   0 0
//   10 10
//   3 4
//   9 7
//   -5 2
// Output:
//   10 1 3
#include <bits/stdc++.h>
using namespace std;

#include "geometry/point.cpp"         // required by geometry/closest-pair (inserted with it)
#include "geometry/closest-pair.cpp"  // in a solution: <leader>rl -> geometry/closest-pair

using P = Point<long long>;

int main() {
    int n;
    cin >> n;
    vector<P> pts(n);
    for (auto &q : pts) cin >> q;
    auto [i, j] = closestPair(pts);
    cout << (pts[i] - pts[j]).dist2() << ' ' << i << ' ' << j << '\n';
}
