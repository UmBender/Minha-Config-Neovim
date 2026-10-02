// Title: Convex hull
// Description: Convex hull as indices into the input, counterclockwise (Andrew's monotone chain).
// Usage:
//   vector<int> h = convexHull(pts);         // pts: vector<Point<T>>, not modified
//   vector<int> h = convexHull(pts, true);   // also keep the points lying on the hull's edges
//   counterclockwise from the smallest point (by x, then y); duplicates appear once (lowest index)
//   all points on a line: its two ends (or every point on it, in order, with `true`); one point: {i}
//   exact with integer T (|coordinates| <= ~1e9); floating T uses sgn's EPS
// Complexity: O(n log n).
// Verify: https://judge.yosupo.jp/problem/static_convex_hull
// Requires: geometry/point
template <class T> vector<int> convexHull(const vector<Point<T>> &pts, bool collinear = false) {
    vector<int> id(pts.size());
    iota(id.begin(), id.end(), 0);
    stable_sort(id.begin(), id.end(), [&](int i, int j) { return pts[i] < pts[j]; });
    id.erase(unique(id.begin(), id.end(), [&](int i, int j) { return pts[i] == pts[j]; }), id.end());
    int n = (int)id.size();
    if (n < 3) return id;
    bool line = true;
    for (int i = 2; i < n && line; i++) line = sgn(pts[id[0]].cross(pts[id[1]], pts[id[i]])) == 0;
    if (line) return collinear ? id : vector<int>{id[0], id[n - 1]};
    // lower chain left to right, then upper chain right to left; pop on right turns (and on
    // collinear points unless they are kept)
    vector<int> h;
    for (int pass = 0; pass < 2; pass++) {
        size_t base = h.size();
        for (int i : id) {
            while (h.size() >= base + 2) {
                int s = sgn(pts[h[h.size() - 2]].cross(pts[h.back()], pts[i]));
                if (s > 0 || (s == 0 && collinear)) break;
                h.pop_back();
            }
            h.push_back(i);
        }
        h.pop_back();  // the last point starts the other chain
        reverse(id.begin(), id.end());
    }
    return h;
}
