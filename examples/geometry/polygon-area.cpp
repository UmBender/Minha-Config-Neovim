// Problem: the area of a polygon (vertices in order, either orientation) and its orientation.
// Input:
//   4
//   1 1
//   4 2
//   3 5
//   1 4
// Output:
//   8 CCW
#include <bits/stdc++.h>
using namespace std;

#include "geometry/point.cpp"         // required by geometry/polygon-area (inserted with it)
#include "geometry/polygon-area.cpp"  // in a solution: <leader>rl -> geometry/polygon-area

using P = Point<long long>;

int main() {
    int n;
    cin >> n;
    vector<P> p(n);
    for (auto &q : p) cin >> q;
    long long a2 = polyArea2(p);  // exact: twice the area
    cout << llabs(a2) / 2 << (a2 % 2 ? ".5" : "") << ' ' << (a2 > 0 ? "CCW" : "CW") << '\n';
}
