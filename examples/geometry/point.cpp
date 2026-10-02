// Problem: for each triple of points a b c, say whether a -> b -> c turns LEFT, RIGHT or goes
//   STRAIGHT, and print the angle at b in degrees.
// Input:
//   3
//   0 0 2 0 2 3
//   0 0 2 0 3 -1
//   1 1 2 2 4 4
// Output:
//   LEFT 90.00
//   RIGHT 135.00
//   STRAIGHT 180.00
#include <bits/stdc++.h>
using namespace std;

#include "geometry/point.cpp"  // in a solution: <leader>rl -> geometry/point

using P = Point<long long>;

int main() {
    int q;
    cin >> q;
    cout << fixed << setprecision(2);
    while (q--) {
        P a, b, c;
        cin >> a >> b >> c;
        int s = sgn(a.cross(b, c));  // exact on integers
        // angle between b -> a and b -> c from the dot and cross products
        P u = a - b, v = c - b;
        long double deg = atan2l(fabsl((long double)u.cross(v)), (long double)u.dot(v)) * 180 / acosl(-1);
        cout << (s > 0 ? "LEFT" : s < 0 ? "RIGHT" : "STRAIGHT") << ' ' << deg << '\n';
    }
}
