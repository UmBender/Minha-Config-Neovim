// Problem: for each pair of segments, print TOUCH if they share any point (CROSS if they cross at
//   one point inside both), or NO.
// Input:
//   4
//   0 0 4 4 0 4 4 0
//   0 0 4 0 4 0 4 3
//   0 0 2 0 1 0 3 0
//   0 0 1 1 2 2 3 3
// Output:
//   CROSS
//   TOUCH
//   TOUCH
//   NO
#include <bits/stdc++.h>
using namespace std;

#include "geometry/point.cpp"                 // required by geometry/segment-intersection
#include "geometry/segment-intersection.cpp"  // in a solution: <leader>rl -> geometry/segment-intersection

using P = Point<long long>;

int main() {
    int q;
    cin >> q;
    while (q--) {
        P a, b, c, d;
        cin >> a >> b >> c >> d;
        cout << (properInter(a, b, c, d) ? "CROSS" : segInter(a, b, c, d) ? "TOUCH" : "NO") << '\n';
    }
}
