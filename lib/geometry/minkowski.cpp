// Title: Minkowski sum
// Description: Minkowski sum {a + b} of two convex polygons, merging their edges by angle.
// Usage:
//   vector<P> c = minkowski(a, b);   // a, b: convex, counterclockwise, any starting vertex,
//                                    // no repeated vertices; a point or a segment ({p, q}) also works
//   c: counterclockwise from its smallest point (by x, then y), without collinear vertices
//   distance between convex polygons A and B: from the origin to minkowski(A, -B) (0 if inside)
// Complexity: O(|a| + |b|).
// Verify: https://judge.yosupo.jp/problem/minkowski_sum_of_convex_polygons
// Requires: geometry/point
template <class T> vector<Point<T>> minkowski(vector<Point<T>> a, vector<Point<T>> b) {
    int n = (int)a.size(), m = (int)b.size();
    // start at the smallest vertex, then repeat the first two at the end (edges i -> i + 1 wrap)
    for (auto *p : {&a, &b}) {
        rotate(p->begin(), min_element(p->begin(), p->end()), p->end());
        for (int i = 0, k = (int)p->size(); i < 2; i++) {
            Point<T> q = (*p)[i % k];
            p->push_back(q);
        }
    }
    // edges from the smallest vertex turn from angle (-pi/2, pi/2] (low) to (pi/2, 3pi/2] (high);
    // z > 0: a's edge comes first, 0: same direction (cross alone can't tell opposite edges apart)
    auto high = [](Point<T> v) { return sgn(v.x) < 0 || (sgn(v.x) == 0 && sgn(v.y) < 0); };
    vector<Point<T>> c;
    for (int i = 0, j = 0; i < n || j < m;) {
        c.push_back(a[i] + b[j]);
        Point<T> ea = a[i + 1] - a[i], eb = b[j + 1] - b[j];
        int z = high(ea) != high(eb) ? (high(eb) ? 1 : -1) : sgn(ea.cross(eb));
        bool ai = j == m || (i < n && z >= 0), bj = i == n || (j < m && z <= 0);
        i += ai, j += bj;
    }
    // drop vertices in the middle of straight edges
    vector<Point<T>> h;
    auto straight = [](Point<T> x, Point<T> y, Point<T> z) { return sgn(x.cross(y, z)) == 0; };
    for (Point<T> q : c) {
        while (h.size() >= 2 && straight(h[h.size() - 2], h.back(), q)) h.pop_back();
        h.push_back(q);
    }
    while (h.size() >= 3 && straight(h[h.size() - 2], h.back(), h[0])) h.pop_back();
    return h;
}
