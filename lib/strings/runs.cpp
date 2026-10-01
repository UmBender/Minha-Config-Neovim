// Title: Runs
// Description: All maximal repetitions (runs) of a string, with their smallest period.
// Usage:
//   vector<array<int, 3>> r = runs(s);   // {p, l, r}, sorted by (p, l)
//   s[l, r) has smallest period p, r - l >= 2p, and can't be extended keeping period p.
//   Every repetition (square, cube, ...) lies inside a run of the same period; there are O(n) runs.
//   s: string or any vector<T> (only == is used)
// Complexity: O(n log n).
// Verify: https://judge.yosupo.jp/problem/runenumerate
// Requires: strings/z-function
template <class S> vector<array<int, 3>> runs(const S &s) {
    int n = (int)s.size();
    vector<vector<pair<int, int>>> byP(n + 1);
    auto cat = [](const S &a, const S &b, const S &c) {
        S res = a;
        res.insert(res.end(), b.begin(), b.end()), res.insert(res.end(), c.begin(), c.end());
        return res;
    };
    // runs that cross the middle M of l + r with period p, with the repeated part starting in l
    // (o = 0) or, on the reversed halves, in r (o = 1)
    auto cross = [&](const S &l, const S &r, int M, int o) {
        int nl = (int)l.size(), nr = (int)r.size();
        S rl(l.rbegin(), l.rend());
        auto z = zFunction(rl), w = zFunction(cat(r, l, r));
        for (int p = 1; p <= nl; p++) {
            int a = p == nl ? nl : min(p + z[p], nl), b = min(w[nl + nr - p], nr);
            if (a + b >= 2 * p) byP[p].emplace_back(M - (o ? b : a), M + (o ? a : b));
        }
    };
    auto rec = [&](auto &&self, int L, int R) -> void {
        if (R - L < 2) return;
        int M = (L + R) / 2;
        self(self, L, M), self(self, M, R);
        S l(s.begin() + L, s.begin() + M), r(s.begin() + M, s.begin() + R);
        cross(l, r, M, 0);
        reverse(l.begin(), l.end()), reverse(r.begin(), r.end());
        cross(r, l, M, 1);
    };
    rec(rec, 0, n);
    vector<array<int, 3>> res;
    set<pair<int, int>> done;  // an interval is reported once, with its smallest period
    for (int p = 1; p <= n; p++) {
        auto &v = byP[p];
        sort(v.begin(), v.end(), [](auto x, auto y) { return x.first != y.first ? x.first < y.first : x.second > y.second; });
        int R = -1;
        for (auto [l, r] : v)
            if (R < r) {  // skip intervals inside an earlier one (not maximal)
                R = r;
                if (done.emplace(l, r).second) res.push_back({p, l, r});
            }
    }
    return res;
}
