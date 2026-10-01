// Title: Lazy segment tree (add, min)
// Description: Range add, range minimum. Plain recursive code, no lambdas.
// Usage:
//   RangeAddMin<long long> seg(n);  RangeAddMin<long long> seg(a);   // zeros / from a vector
//   seg.add(l, r, x)   add x to a[l, r)
//   seg.min(l, r)      minimum of [l, r), requires l < r
//   seg.set(i, x);  seg.get(i);
// Complexity: O(log n) per operation, O(n) build.
template <class T = long long> struct RangeAddMin {
    static constexpr T E = numeric_limits<T>::max();  // minimum of an empty range
    int n;
    vector<T> v, lz;  // v[i]: minimum of node i, lz[i]: add still to push to its children
    RangeAddMin(int n_ = 0) : RangeAddMin(vector<T>(n_, T{})) {}
    RangeAddMin(const vector<T> &a) : n((int)a.size()), v(4 * std::max(n, 1)), lz(4 * std::max(n, 1)) {
        if (n) build(1, 0, n, a);
    }
    void build(int i, int l, int r, const vector<T> &a) {
        if (r - l == 1) return void(v[i] = a[l]);
        int m = (l + r) / 2;
        build(2 * i, l, m, a), build(2 * i + 1, m, r, a);
        pull(i);
    }
    void pull(int i) { v[i] = std::min(v[2 * i], v[2 * i + 1]); }
    void apply(int i, T x) { v[i] += x, lz[i] += x; }
    void push(int i) { apply(2 * i, lz[i]), apply(2 * i + 1, lz[i]), lz[i] = T{}; }
    void add(int ql, int qr, T x) { add(ql, qr, x, 1, 0, n); }
    void add(int ql, int qr, T x, int i, int l, int r) {
        if (qr <= l || r <= ql) return;
        if (ql <= l && r <= qr) return apply(i, x);
        int m = (l + r) / 2;
        push(i);
        add(ql, qr, x, 2 * i, l, m), add(ql, qr, x, 2 * i + 1, m, r);
        pull(i);
    }
    T min(int ql, int qr) { return min(ql, qr, 1, 0, n); }
    T min(int ql, int qr, int i, int l, int r) {
        if (qr <= l || r <= ql) return E;
        if (ql <= l && r <= qr) return v[i];
        int m = (l + r) / 2;
        push(i);
        return std::min(min(ql, qr, 2 * i, l, m), min(ql, qr, 2 * i + 1, m, r));
    }
    T get(int i) { return min(i, i + 1); }
    void set(int i, T x) { add(i, i + 1, x - get(i)); }
};
