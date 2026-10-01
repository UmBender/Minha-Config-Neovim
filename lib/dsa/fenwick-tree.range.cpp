// Title: Fenwick tree (range add, range sum)
// Description: Range add and range sum with two plain Fenwick arrays, 0-indexed.
// Usage:
//   RangeFenwick<long long> rf(n);  RangeFenwick<long long> rf(a);   // zeros / from a vector
//   rf.add(l, r, x)    add x to a[l, r)
//   rf.sum(r)          sum of [0, r)        rf.sum(l, r)   sum of [l, r)
//   rf.get(i);  rf.set(i, x);
// Complexity: O(log n) per operation, O(n log n) build from a vector.
template <class T = long long> struct RangeFenwick {
    // a[i] = d[0] + ... + d[i] (d: difference array). Then
    //   sum(r) = sum_{i < r} (r - i) d[i] = r * sum_{i < r} d[i] - sum_{i < r} i d[i],
    // so two Fenwick trees over d[i] and i d[i] answer prefix sums.
    int n;
    vector<T> b1, b2;
    RangeFenwick(int n_ = 0) : n(n_), b1(n_, T{}), b2(n_, T{}) {}
    RangeFenwick(const vector<T> &a) : RangeFenwick((int)a.size()) {
        for (int i = 0; i < n; i++) add(i, i + 1, a[i]);
    }
    void upd(int i, T x) {
        for (int j = i + 1; j <= n; j += j & -j) b1[j - 1] += x, b2[j - 1] += x * (T)i;
    }
    void add(int l, int r, T x) {
        if (l < r) upd(l, x), upd(r, -x);
    }
    T sum(int r) const {
        T s1{}, s2{};
        for (int j = r; j > 0; j -= j & -j) s1 += b1[j - 1], s2 += b2[j - 1];
        return s1 * (T)r - s2;
    }
    T sum(int l, int r) const { return sum(r) - sum(l); }
    T get(int i) const { return sum(i, i + 1); }
    void set(int i, T x) { add(i, i + 1, x - get(i)); }
};
