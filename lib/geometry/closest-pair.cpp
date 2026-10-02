// Title: Closest pair of points
// Description: Indices of two points at the smallest Euclidean distance (sweep line over x).
// Usage:
//   auto [i, j] = closestPair(pts);          // pts: vector<Point<T>>, size >= 2; i < j
//   distance: (pts[i] - pts[j]).dist(), squared (exact for integers): (pts[i] - pts[j]).dist2()
//   repeated points are fine (distance 0); integer |coordinates| <= ~1e9
// Complexity: O(n log n).
// Verify: https://judge.yosupo.jp/problem/closest_pair
// Requires: geometry/point
template <class T> pair<int, int> closestPair(const vector<Point<T>> &pts) {
    int n = (int)pts.size();
    vector<int> id(n);
    iota(id.begin(), id.end(), 0);
    sort(id.begin(), id.end(), [&](int i, int j) { return pts[i] < pts[j]; });
    // points within the current best distance in x, ordered by y
    set<pair<T, int>> window;
    pair<int, int> best{id[0], id[1]};
    T bestD = (pts[id[0]] - pts[id[1]]).dist2();
    for (int i = 0, l = 0; i < n; i++) {
        Point<T> p = pts[id[i]];
        auto sq = [](T v) { return v * v; };
        while (l < i && sq(p.x - pts[id[l]].x) > bestD) window.erase({pts[id[l]].y, id[l]}), l++;
        T d = (T)sqrtl((long double)bestD);  // integers: |dy| < sqrt(best) means |dy| <= floor of it
        auto it = window.lower_bound({p.y - d, INT_MIN});
        for (; it != window.end() && it->first <= p.y + d; it++)
            if (T cur = (p - pts[it->second]).dist2(); cur < bestD) bestD = cur, best = {it->second, id[i]};
        window.insert({p.y, id[i]});
    }
    if (best.first > best.second) swap(best.first, best.second);
    return best;
}
