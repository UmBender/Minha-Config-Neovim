// Title: Polygon area
// Description: Twice the signed area of a polygon (shoelace): positive when counterclockwise.
// Usage:
//   T a2 = polyArea2(p);     // p: vector<Point<T>>, vertices in order (not repeated at the end)
//   area = abs(a2) / 2.0;  a2 > 0: counterclockwise, a2 < 0: clockwise
//   integer T: exact (twice the area is an integer); lattice points inside (Pick):
//   (abs(a2) - boundary + 2) / 2, boundary = sum of gcd(|dx|, |dy|) over the edges
// Complexity: O(n).
// Requires: geometry/point
template <class T> T polyArea2(const vector<Point<T>> &p) {
    T s = 0;
    for (int i = 0, n = (int)p.size(); i < n; i++) s += p[i].cross(p[(i + 1) % n]);
    return s;
}
