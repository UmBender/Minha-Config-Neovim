// Title: Wavelet matrix
// Description: Static range queries on values: k-th smallest, counts, predecessor/successor, sums, decomposition.
// Usage:
//   WaveletMatrix<long long> wm(a);          // any ordered T; values are compressed internally
//   wm.kth(l, r, k)          k-th smallest (0-indexed) in a[l, r)
//   wm.countLess(l, r, x)    # of a[i] < x in [l, r)        wm.count(l, r, lo, hi)  # with lo <= a[i] < hi
//   wm.prev(l, r, x)         largest value < x  (optional)  wm.next(l, r, x)  smallest value >= x (optional)
//   wm.sum(l, r, lo, hi)     sum of a[i] in [l, r) with lo <= a[i] < hi (arithmetic T; integer sums
//                            wrap internally, exact whenever the answer fits in long long)
//   wm.visit(l, r, lo, hi, f)   calls f(h, s, e) for disjoint ranges [s, e) of level h whose elements
//                               are exactly the a[i], i in [l, r), with lo <= a[i] < hi.
//   wm.pos[h][i]             position of a[i] in level h (h = wm.lg: original order, h = 0: sorted)
//     -> keep one Fenwick per level indexed by position to support weight updates in rectangles.
// Presets:
//   WaveletMatrix wm(a);                     // T deduced from a
//   DistinctCount dc(a);  dc.query(l, r)     // # of distinct values in a[l, r) (any ordered T)
// Complexity: O(n log n) build, O(log n) per query (visit: O(log n) ranges).
// Verify: https://judge.yosupo.jp/problem/range_kth_smallest
// Verify: https://judge.yosupo.jp/problem/static_range_count_distinct
template <class T> struct WaveletMatrix {
    using S = conditional_t<is_integral_v<T>, long long, T>;
    using Acc = conditional_t<is_integral_v<T>, unsigned long long, T>;  // wrapping prefix sums
    int n, lg;
    vector<T> vals;                // sorted distinct values
    vector<vector<int>> zeros;     // zeros[h][i]: # of bit-h zeros among the first i of level h + 1
    vector<int> nz;                // nz[h] = zeros[h][n]
    vector<vector<int>> pos;       // pos[h][i]: position of a[i] in level h
    vector<vector<Acc>> sums;      // sums[h][i]: prefix sums of values in level h order
    WaveletMatrix(const vector<T> &a) : n((int)a.size()), lg(1), vals(a) {
        sort(vals.begin(), vals.end());
        vals.erase(unique(vals.begin(), vals.end()), vals.end());
        while ((1 << lg) < (int)vals.size()) lg++;
        vector<int> cur(n), idx(n);
        for (int i = 0; i < n; i++) cur[i] = int(lower_bound(vals.begin(), vals.end(), a[i]) - vals.begin());
        iota(idx.begin(), idx.end(), 0);
        zeros.assign(lg, vector<int>(n + 1, 0)), nz.assign(lg, 0);
        pos.assign(lg + 1, vector<int>(n)), sums.assign(lg + 1, vector<Acc>(n + 1, Acc{}));
        auto fill = [&](int h) {
            for (int p = 0; p < n; p++) {
                pos[h][idx[p]] = p;
                if constexpr (is_arithmetic_v<T>) sums[h][p + 1] = sums[h][p] + (Acc)vals[cur[p]];
            }
        };
        fill(lg);
        for (int h = lg - 1; h >= 0; h--) {
            for (int i = 0; i < n; i++) zeros[h][i + 1] = zeros[h][i] + !(cur[i] >> h & 1);
            nz[h] = zeros[h][n];
            vector<int> nc, ni;
            for (int bit = 0; bit < 2; bit++)
                for (int i = 0; i < n; i++)
                    if ((cur[i] >> h & 1) == bit) nc.push_back(cur[i]), ni.push_back(idx[i]);
            cur = nc, idx = ni;
            fill(h);
        }
    }
    int index(const T &x) const { return int(lower_bound(vals.begin(), vals.end(), x) - vals.begin()); }
    T kth(int l, int r, int k) const {
        int res = 0;
        for (int h = lg - 1; h >= 0; h--) {
            int zl = zeros[h][l], zr = zeros[h][r];
            if (k < zr - zl) l = zl, r = zr;
            else k -= zr - zl, res |= 1 << h, l = nz[h] + l - zl, r = nz[h] + r - zr;
        }
        return vals[res];
    }
    // (count, sum) of elements in [l, r) whose compressed value is < c
    pair<int, Acc> lessIndex(int l, int r, int c) const {
        if (c >= (1 << lg)) return {r - l, sums[lg][r] - sums[lg][l]};
        int cnt = 0;
        Acc s{};
        for (int h = lg - 1; h >= 0; h--) {
            int zl = zeros[h][l], zr = zeros[h][r];
            if (c >> h & 1) {
                cnt += zr - zl, s += sums[h][zr] - sums[h][zl];
                l = nz[h] + l - zl, r = nz[h] + r - zr;
            } else {
                l = zl, r = zr;
            }
        }
        return {cnt, s};
    }
    int countLess(int l, int r, const T &x) const { return lessIndex(l, r, index(x)).first; }
    int count(int l, int r, const T &lo, const T &hi) const {
        return lo < hi ? countLess(l, r, hi) - countLess(l, r, lo) : 0;
    }
    S sum(int l, int r, const T &lo, const T &hi) const {
        return lo < hi ? (S)(lessIndex(l, r, index(hi)).second - lessIndex(l, r, index(lo)).second) : S{};
    }
    optional<T> prev(int l, int r, const T &x) const {
        int c = countLess(l, r, x);
        return c ? optional<T>(kth(l, r, c - 1)) : nullopt;
    }
    optional<T> next(int l, int r, const T &x) const {
        int c = countLess(l, r, x);
        return c < r - l ? optional<T>(kth(l, r, c)) : nullopt;
    }
    template <class F> void visit(int l, int r, const T &lo, const T &hi, F f) const {
        int cl = index(lo), ch = index(hi);
        auto rec = [&](auto self, int h, int s, int e, int prefix) -> void {
            if (s >= e || prefix + (1 << h) <= cl || ch <= prefix) return;
            if (cl <= prefix && prefix + (1 << h) <= ch) {
                f(h, s, e);
                return;
            }
            int zs = zeros[h - 1][s], ze = zeros[h - 1][e];
            self(self, h - 1, zs, ze, prefix);
            self(self, h - 1, nz[h - 1] + s - zs, nz[h - 1] + e - ze, prefix + (1 << (h - 1)));
        };
        if (cl < ch) rec(rec, lg, l, r, 0);
    }
};

// distinct values in a[l, r) = # of i in [l, r) whose previous occurrence is before l
struct DistinctCount {
    WaveletMatrix<int> wm;
    template <class T> static vector<int> prevOccurrence(const vector<T> &a) {
        map<T, int> last;
        vector<int> prv(a.size());
        for (int i = 0; i < (int)a.size(); i++) {
            auto it = last.find(a[i]);
            prv[i] = it == last.end() ? -1 : it->second;
            last[a[i]] = i;
        }
        return prv;
    }
    template <class T> DistinctCount(const vector<T> &a) : wm(prevOccurrence(a)) {}
    int query(int l, int r) const { return wm.countLess(l, r, l); }
};
