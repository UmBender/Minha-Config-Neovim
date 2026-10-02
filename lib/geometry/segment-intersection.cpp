// Title: Segment intersection
// Description: Do two closed segments touch (segInter) or cross at one interior point (properInter); onSegment.
// Usage:
//   segInter(a, b, c, d)     // segments ab and cd share at least one point (endpoints, overlaps,
//                            // degenerate segments a == b included)
//   properInter(a, b, c, d)  // they cross at a single point interior to both
//   onSegment(p, a, b)       // p lies on the closed segment ab
//   exact with integer T (|coordinates| <= ~1e9); floating T uses sgn's EPS
//   the crossing point itself: lineInter (geometry/line-intersection) when properInter holds
// Complexity: O(1).
// Requires: geometry/point
template <class T> bool onSegment(Point<T> p, Point<T> a, Point<T> b) {
    return sgn(a.cross(b, p)) == 0 && sgn((a - p).dot(b - p)) <= 0;
}

template <class T> bool properInter(Point<T> a, Point<T> b, Point<T> c, Point<T> d) {
    int oa = sgn(c.cross(d, a)), ob = sgn(c.cross(d, b)), oc = sgn(a.cross(b, c)), od = sgn(a.cross(b, d));
    return oa * ob < 0 && oc * od < 0;
}

template <class T> bool segInter(Point<T> a, Point<T> b, Point<T> c, Point<T> d) {
    return properInter(a, b, c, d) || onSegment(c, a, b) || onSegment(d, a, b) || onSegment(a, c, d) ||
           onSegment(b, c, d);
}
