// Title: Polygon cut
// Description: Part of a polygon on the left of the directed line a -> b (Sutherland-Hodgman, one half-plane).
// Usage:
//   using P = Point<long double>;            // new vertices are built: floating T
//   vector<P> q = polygonCut(p, a, b);       // p: vertices in order; keeps cross(b - a, x - a) >= 0
//   the line is infinite; q keeps p's order and orientation; q.size() < 3: nothing (or only a
//   segment / point on the line) is left
//   convex p: q is p intersected with the half-plane; the other side: polygonCut(p, b, a)
//   non-convex p: the pieces stay joined by zero-width edges along the line (areas still add up)
//   half-plane intersection of a few planes: cut a big box by each line (O(n) per cut)
// Complexity: O(n).
// Requires: geometry/point
template <class T> vector<Point<T>> polygonCut(const vector<Point<T>> &p, Point<T> a, Point<T> b) {
    vector<Point<T>> res;
    for (int i = 0, n = (int)p.size(); i < n; i++) {
        Point<T> c = p[i], d = p[(i + 1) % n];
        T sc = a.cross(b, c), sd = a.cross(b, d);
        if (sgn(sc) >= 0) res.push_back(c);
        if (sgn(sc) * sgn(sd) < 0) res.push_back(c + (d - c) * (sc / (sc - sd)));  // edge crosses the line
    }
    return res;
}
