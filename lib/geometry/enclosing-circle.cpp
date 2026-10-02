// Title: Minimum enclosing circle
// Description: Smallest circle containing every point (Welzl, randomized incremental).
// Usage:
//   using P = Point<long double>;
//   auto [o, r] = enclosingCircle(pts);   // pts: vector<P>, non-empty, not modified; center o, radius r
//   points within r + EPS of o count as inside (tune EPS for large coordinates)
// Complexity: O(n) expected.
// Verify: https://judge.yosupo.jp/problem/minimum_enclosing_circle
// Requires: geometry/point
template <class T> pair<Point<T>, T> enclosingCircle(vector<Point<T>> p) {
    using C = pair<Point<T>, T>;
    shuffle(p.begin(), p.end(), mt19937((unsigned)chrono::steady_clock::now().time_since_epoch().count()));
    auto in = [](const C &c, Point<T> q) { return (q - c.first).dist() <= c.second + EPS; };
    auto two = [](Point<T> a, Point<T> b) { return C{(a + b) / 2, T((a - b).dist() / 2)}; };
    auto three = [&](Point<T> a, Point<T> b, Point<T> c) {
        Point<T> u = b - a, v = c - a;
        T d = 2 * u.cross(v);
        if (sgn(d) == 0) {  // collinear (only through rounding): the widest pair
            C x = two(a, b), y = two(a, c), z = two(b, c);
            return x.second >= y.second && x.second >= z.second ? x : y.second >= z.second ? y : z;
        }
        Point<T> o = a + (u * v.dist2() - v * u.dist2()).perp() / d;  // circumcenter
        return C{o, T((o - a).dist())};
    };
    C c{p[0], 0};
    // invariant: c is the smallest circle of p[0..i] with the fixed points (p[i], then p[j]) on it
    for (int i = 1; i < (int)p.size(); i++)
        if (!in(c, p[i])) {
            c = {p[i], 0};
            for (int j = 0; j < i; j++)
                if (!in(c, p[j])) {
                    c = two(p[i], p[j]);
                    for (int k = 0; k < j; k++)
                        if (!in(c, p[k])) c = three(p[i], p[j], p[k]);
                }
        }
    return c;
}
