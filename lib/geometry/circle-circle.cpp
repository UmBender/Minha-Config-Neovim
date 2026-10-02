// Title: Circle-circle intersection
// Description: Intersection points (0, 1 or 2) of two circles.
// Usage:
//   using P = Point<long double>;
//   vector<P> v = circleCircle(c1, r1, c2, r2);   // centers c1, c2, radii r1, r2
//   v.size(): 0 apart, nested or concentric (the same circle too: check c1 == c2 && r1 == r2
//   first if it matters), 1 touching, 2 crossing (v[0] to the right of c1 -> c2, v[1] to the left)
//   tangency is decided by sgn on squared lengths (EPS)
// Complexity: O(1).
// Requires: geometry/point
template <class T> vector<Point<T>> circleCircle(Point<T> c1, T r1, Point<T> c2, T r2) {
    Point<T> d = c2 - c1;
    T d2 = d.dist2();
    if (sgn(d2) == 0) return {};
    // the common chord crosses c1 -> c2 at c1 + d * a, at half-length sqrt(h2)
    T a = (d2 + r1 * r1 - r2 * r2) / (2 * d2), h2 = r1 * r1 - a * a * d2;
    int s = sgn(h2);
    if (s < 0) return {};
    Point<T> m = c1 + d * a;
    if (s == 0) return {m};
    Point<T> off = d.perp() * T(sqrtl(h2 / d2));
    return {m - off, m + off};
}
