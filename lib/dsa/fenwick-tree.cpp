// Title: Fenwick tree
// Description: Point add / prefix and range sums over any additive type (BIT), 0-indexed.
// Usage:
//   Fenwick<long long> fw(n);   Fenwick<long long> fw(a);   // zeros / from a vector in O(n)
//   fw.add(i, x);  fw.set(i, x);  fw.get(i);
//   fw.sum(r)      sum of [0, r)        fw.sum(l, r)   sum of [l, r)
//   fw.lowerBound(s)  smallest r with sum(r) >= s (all values >= 0); n + 1 if none, 0 if s <= 0
// Presets:
//   Fenwick fw(n);                  // T defaults to long long
//   RangeFenwick rf(n or a);        // range add + range sum (long long by default)
//   rf.add(l, r, x);  rf.sum(l, r);  rf.sum(r);  rf.get(i);  rf.set(i, x)
// Complexity: O(log n) per operation, O(n) build.
// Verify: https://judge.yosupo.jp/problem/point_add_range_sum
template <class T = long long> struct Fenwick {
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

// ---- presets ----
template <class T = long long> struct RangeFenwick {
    Fenwick<T> b1, b2;  // prefix sum(r) = b1.sum(r) * r - b2.sum(r)
    static vector<T> neg(vector<T> a) {
        for (T &x : a) x = -x;
        return a;
    }
    RangeFenwick(int n = 0) : b1(n), b2(n) {}
    RangeFenwick(const vector<T> &a) : b1((int)a.size()), b2(neg(a)) {}
    void add(int l, int r, T x) {
        b1.add(l, x), b1.add(r, -x);
        b2.add(l, x * (T)l), b2.add(r, -x * (T)r);
    }
    T sum(int r) const { return b1.sum(r) * (T)r - b2.sum(r); }
    T sum(int l, int r) const { return sum(r) - sum(l); }
    T get(int i) const { return sum(i, i + 1); }
    void set(int i, T x) { add(i, i + 1, x - get(i)); }
};
