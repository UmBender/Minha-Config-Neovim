// Title: Fenwick tree
// Description: Point add / prefix and range sums over any additive type (BIT), 0-indexed.
// Usage:
//   Fenwick<long long> fw(n);   Fenwick<long long> fw(a);   // zeros / from a vector in O(n)
//   fw.add(i, x);  fw.set(i, x);  fw.get(i);
//   fw.sum(r)      sum of [0, r)        fw.sum(l, r)   sum of [l, r)
//   fw.lowerBound(s)  smallest r with sum(r) >= s (all values >= 0); n + 1 if none, 0 if s <= 0
// Complexity: O(log n) per operation, O(n) build.
// Verify: https://judge.yosupo.jp/problem/point_add_range_sum
// Pending: example + presets (T-009..T-011), remove when done
template <class T> struct Fenwick {
    int n;
    vector<T> t;
    Fenwick(int n_ = 0) : n(n_), t(n_, T{}) {}
    Fenwick(const vector<T> &a) : n((int)a.size()), t(a) {
        for (int i = 1; i <= n; i++) {
            int j = i + (i & -i);
            if (j <= n) t[j - 1] += t[i - 1];
        }
    }
    void add(int i, T x) {
        for (i++; i <= n; i += i & -i) t[i - 1] += x;
    }
    T sum(int r) const {
        T s{};
        for (; r > 0; r -= r & -r) s += t[r - 1];
        return s;
    }
    T sum(int l, int r) const { return sum(r) - sum(l); }
    T get(int i) const { return sum(i, i + 1); }
    void set(int i, T x) { add(i, x - get(i)); }
    int lowerBound(T s) const {
        if (!(T{} < s)) return 0;
        int pos = 0, pw = 1;
        while (pw * 2 <= n) pw *= 2;
        for (; pw; pw /= 2)
            if (pos + pw <= n && t[pos + pw - 1] < s) pos += pw, s -= t[pos - 1];
        return pos + 1;
    }
};
