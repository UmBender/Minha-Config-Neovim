// Title: Circle-line intersection
// Description: Intersection points (0, 1 or 2) of a circle and the infinite line through two points.
// Usage:
//   using P = Point<long double>;
//   vector<P> v = circleLine(c, r, a, b);   // circle: center c, radius r; line through a != b
//   v.size(): 0 apart, 1 tangent, 2 crossing (ordered in the direction a -> b)
//   a segment instead of the line: keep the points with (p - a).dot(p - b) <= 0
//   tangency is decided by sgn on r^2 - dist^2 (EPS on squared lengths)
// Complexity: O(1).
// Requires: geometry/point
template <class T> vector<Point<T>> circleLine(Point<T> c, T r, Point<T> a, Point<T> b) {
    Point<T> d = b - a, f = a + d * ((c - a).dot(d) / d.dist2());  // f: foot of the perpendicular from c
    T h2 = r * r - (c - f).dist2();
    int s = sgn(h2);
    if (s < 0) return {};
    if (s == 0) return {f};
    Point<T> off = d * T(sqrtl(h2) / d.dist());
    return {f - off, f + off};
}
