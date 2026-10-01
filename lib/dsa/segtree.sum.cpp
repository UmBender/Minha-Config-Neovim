// Title: Segment tree (sum)
// Description: Point set/add, range sum. Plain bottom-up code, no lambdas.
// Usage:
//   SumSegtree<long long> seg(n);  SumSegtree<long long> seg(a);   // zeros / from a vector
//   seg.set(i, x);  seg.add(i, x);  seg.get(i);
//   seg.query(l, r)   sum of [l, r) (0 if empty)
// Complexity: O(log n) per operation, O(n) build.
template <class T = long long> struct SumSegtree {
    int n;
    vector<T> t;  // leaves at [n, 2n), node i = t[2i] + t[2i + 1]
    SumSegtree(int n_ = 0) : n(n_), t(2 * n_, T{}) {}
    SumSegtree(const vector<T> &a) : n((int)a.size()), t(2 * a.size()) {
        copy(a.begin(), a.end(), t.begin() + n);
        for (int i = n - 1; i >= 1; i--) t[i] = t[2 * i] + t[2 * i + 1];
    }
    void set(int i, T x) {
        for (t[i += n] = x; i >>= 1;) t[i] = t[2 * i] + t[2 * i + 1];
    }
    void add(int i, T x) { set(i, t[i + n] + x); }
    T get(int i) const { return t[i + n]; }
    T query(int l, int r) const {
        T s{};
        for (l += n, r += n; l < r; l >>= 1, r >>= 1) {
            if (l & 1) s += t[l++];
            if (r & 1) s += t[--r];
        }
        return s;
    }
};
