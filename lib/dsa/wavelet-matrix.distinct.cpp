// Title: Distinct values in a range
// Description: Number of distinct values in a[l, r) of a static array, online (any ordered T).
// Usage:
//   DistinctCount dc(a);   // a: vector<T>, T with operator<
//   dc.query(l, r)         // # of distinct values in a[l, r) (0 if empty)
// Complexity: O(n log n) build and memory, O(log^2 n) per query.
// Verify: https://judge.yosupo.jp/problem/static_range_count_distinct
struct DistinctCount {
    // a value is counted once in [l, r): at its first occurrence i there, i.e. the i in [l, r)
    // whose previous occurrence prv[i] is < l. A merge sort tree over prv counts them.
    int n;
    vector<vector<int>> t;  // bottom-up segment tree; node = sorted prv[] of its elements
    template <class T> DistinctCount(const vector<T> &a) : n((int)a.size()), t(2 * a.size()) {
        map<T, int> last;
        for (int i = 0; i < n; i++) {
            auto [it, fresh] = last.try_emplace(a[i], i);
            t[n + i] = {fresh ? -1 : it->second};
            it->second = i;
        }
        for (int i = n - 1; i >= 1; i--)
            merge(t[2 * i].begin(), t[2 * i].end(), t[2 * i + 1].begin(), t[2 * i + 1].end(), back_inserter(t[i]));
    }
    int query(int l, int r) const {
        int res = 0;
        auto below = [&](const vector<int> &v) { return int(lower_bound(v.begin(), v.end(), l) - v.begin()); };
        for (int lo = l + n, hi = r + n; lo < hi; lo >>= 1, hi >>= 1) {
            if (lo & 1) res += below(t[lo++]);
            if (hi & 1) res += below(t[--hi]);
        }
        return res;
    }
};
