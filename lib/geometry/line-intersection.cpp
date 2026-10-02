// Title: Line intersection
// Description: Intersection of the infinite lines ab and cd, with parallel and same-line cases reported.
// Usage:
//   using P = Point<long double>;            // the point is built by division: floating T
//   auto [k, p] = lineInter(a, b, c, d);     // lines through a, b and through c, d (a != b, c != d)
//   k == 1: one point p;  k == 0: parallel;  k == -1: the same line (p is meaningless when k != 1)
//   segments, not lines: check segInter (geometry/segment-intersection) first
// Complexity: O(1).
// Requires: geometry/point
template <class T> pair<int, Point<T>> lineInter(Point<T> a, Point<T> b, Point<T> c, Point<T> d) {
    T den = (b - a).cross(d - c);
    if (sgn(den) == 0) return {sgn(a.cross(b, c)) == 0 ? -1 : 0, {}};
    return {1, a + (b - a) * ((c - a).cross(d - c) / den)};
}
