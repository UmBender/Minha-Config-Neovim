// Title: Segment tree (max)
// Description: Point set, range maximum. Plain bottom-up code, no lambdas.
// Usage:
//   MaxSegtree<long long> seg(a);   MaxSegtree<long long> seg(n);   // from a vector / n empty slots
//   seg.set(i, x);  seg.get(i);
//   seg.query(l, r)   maximum of [l, r); MaxSegtree<T>::E (numeric_limits<T>::lowest()) if empty
// Complexity: O(log n) per operation, O(n) build.
template <class T = long long> struct MaxSegtree {
    static constexpr T E = numeric_limits<T>::lowest();
    int n;
    vector<T> t;  // leaves at [n, 2n), node i = max(t[2i], t[2i + 1])
    MaxSegtree(int n_ = 0) : n(n_), t(2 * n_, E) {}
    MaxSegtree(const vector<T> &a) : n((int)a.size()), t(2 * a.size(), E) {
        copy(a.begin(), a.end(), t.begin() + n);
        for (int i = n - 1; i >= 1; i--) t[i] = max(t[2 * i], t[2 * i + 1]);
    }
    void set(int i, T x) {
        for (t[i += n] = x; i >>= 1;) t[i] = max(t[2 * i], t[2 * i + 1]);
    }
    T get(int i) const { return t[i + n]; }
    T query(int l, int r) const {
        T s = E;
        for (l += n, r += n; l < r; l >>= 1, r >>= 1) {
            if (l & 1) s = max(s, t[l++]);
            if (r & 1) s = max(s, t[--r]);
        }
        return s;
    }
};
