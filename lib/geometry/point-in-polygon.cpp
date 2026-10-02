// Title: Point in polygon
// Description: Inside / border / outside test for a convex polygon in O(log n) or any simple polygon in O(n).
// Usage:
//   int s = inConvex(p, q);    // p: convex, counterclockwise, any starting vertex; collinear vertices
//                              // OK if p[0] is a corner (e.g. convexHull(pts, true) output);
//                              // n >= 3 needs a positive area, n <= 2 is a point or a segment
//   int s = inPolygon(p, q);   // p: any simple polygon, either orientation
//   s == 1: strictly inside, 0: on the border, -1: outside  (s >= 0: inside or on the border)
//   exact with integer T; vertices not repeated at the end
// Complexity: inConvex O(log n), inPolygon O(n).
// Verify: https://codeforces.com/gym/101848/problem/A
// Requires: geometry/point
template <class T> int inConvex(const vector<Point<T>> &p, Point<T> q) {
    int n = (int)p.size();
    auto on = [&](Point<T> a, Point<T> b) {  // q on the closed segment ab
        return sgn(a.cross(b, q)) == 0 && sgn((a - q).dot(b - q)) <= 0;
    };
    if (n < 3) return n && on(p[0], p[n - 1]) ? 0 : -1;
    // the wedge at p[0] between p[1] and p[n - 1], then the triangle p[0] p[l] p[l + 1] holding q
    if (sgn(p[0].cross(p[1], q)) < 0 || sgn(p[0].cross(p[n - 1], q)) > 0) return -1;
    if (on(p[0], p[0])) return 0;
    // last l with q left of p[0] -> p[l]; on the ray p[0] -> p[l] counts only for the first side, so
    // that q on the last side's line ends up in the triangle before that side's farthest vertex
    bool first = sgn(p[0].cross(p[1], q)) == 0;
    int l = 1, r = n - 1;
    while (r - l > 1) {
        int m = (l + r) / 2, c = sgn(p[0].cross(p[m], q));
        (c > 0 || (c == 0 && first) ? l : r) = m;
    }
    int s = sgn(p[l].cross(p[l + 1], q));
    if (s <= 0) return s < 0 || !on(p[l], p[l + 1]) ? -1 : 0;
    // inside the triangle: on the border only on its sides from p[0] that are polygon edges (the
    // chains of vertices collinear with p[0] at either end)
    if (sgn(p[0].cross(p[l], q)) == 0 && sgn(p[0].cross(p[1], p[l])) == 0) return 0;
    if (sgn(p[0].cross(p[l + 1], q)) == 0 && sgn(p[0].cross(p[n - 1], p[l + 1])) == 0) return 0;
    return 1;
}

template <class T> int inPolygon(const vector<Point<T>> &p, Point<T> q) {
    bool in = false;
    for (int i = 0, n = (int)p.size(); i < n; i++) {
        Point<T> a = p[i], b = p[(i + 1) % n];
        int s = sgn(a.cross(b, q));
        if (s == 0 && sgn((a - q).dot(b - q)) <= 0) return 0;
        // a ray from q to +x crosses edges that straddle its line (half-open in y) with q to their left
        // when going up, to their right when going down
        if ((a.y <= q.y) != (b.y <= q.y) && (b.y > a.y) == (s > 0)) in = !in;
    }
    return in ? 1 : -1;
}
