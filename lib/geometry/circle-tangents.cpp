// Title: Circle tangents
// Description: Common tangent lines of two circles (or tangents from a point), as pairs of touching points.
// Usage:
//   using P = Point<long double>;
//   auto t = circleTangents(c1, r1, c2, r2);   // vector<pair<P, P>> {point on circle 1, point on circle 2}
//   distinct lines only: 4 apart, 3 touching outside, 2 crossing, 1 touching inside, 0 nested or
//   concentric; the outer tangents come first (both circles on the same side)
//   tangents from a point q to a circle: circleTangents(c, r, q, 0) -> 2, 1 (q on it) or 0 lines,
//   .first is the touching point, .second == q
//   touching circles: the tangent at the contact point has .first == .second
// Complexity: O(1).
// Requires: geometry/point
template <class T> vector<pair<Point<T>, Point<T>>> circleTangents(Point<T> c1, T r1, Point<T> c2, T r2) {
    vector<pair<Point<T>, Point<T>>> res;
    Point<T> d = c2 - c1;
    T d2 = d.dist2();
    if (sgn(d2) == 0) return res;
    for (int s : {1, -1}) {  // outer (s = 1), inner (s = -1): circle 2 seen with radius s * r2
        if (s == -1 && sgn(r2) == 0) break;  // a point: inner tangents are the outer ones
        T dr = r1 - s * r2, h2 = d2 - dr * dr;
        int k = sgn(h2);
        if (k < 0) continue;
        T h = T(sqrtl(max(h2, T(0))));
        for (int t : {1, -1}) {
            Point<T> n = (d * dr + d.perp() * (t * h)) / d2;  // unit normal of the tangent line
            res.push_back({c1 + n * r1, c2 + n * (s * r2)});
            if (k == 0) break;  // touching: one line
        }
    }
    return res;
}
