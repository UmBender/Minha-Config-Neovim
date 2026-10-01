// Title: Lazy segment tree (assign, max)
// Description: Range assign, range maximum. Plain recursive code, no lambdas.
// Usage:
//   RangeAssignMax<long long> seg(n);  RangeAssignMax<long long> seg(a);   // zeros / from a vector
//   seg.assign(l, r, x)   set every element of a[l, r) to x
//   seg.max(l, r)      maximum of [l, r), requires l < r
//   seg.set(i, x);  seg.get(i);
// Complexity: O(log n) per operation, O(n) build.
template <class T = long long> struct RangeAssignMax {
    static constexpr T E = numeric_limits<T>::lowest();  // maximum of an empty range
    int n;
    vector<T> v, val;  // v[i]: maximum of node i; val[i]: assignment still to push to its children
    vector<char> has;  // has[i]: val[i] is pending
    RangeAssignMax(int n_ = 0) : RangeAssignMax(vector<T>(n_, T{})) {}
    RangeAssignMax(const vector<T> &a)
        : n((int)a.size()), v(4 * std::max(n, 1)), val(4 * std::max(n, 1)), has(4 * std::max(n, 1)) {
        if (n) build(1, 0, n, a);
    }
    void build(int i, int l, int r, const vector<T> &a) {
        if (r - l == 1) return void(v[i] = a[l]);
        int m = (l + r) / 2;
        build(2 * i, l, m, a), build(2 * i + 1, m, r, a);
        pull(i);
    }
    void pull(int i) { v[i] = std::max(v[2 * i], v[2 * i + 1]); }
    void apply(int i, T x) { v[i] = x, val[i] = x, has[i] = 1; }
    void push(int i) {
        if (has[i]) apply(2 * i, val[i]), apply(2 * i + 1, val[i]), has[i] = 0;
    }
    void assign(int ql, int qr, T x) { assign(ql, qr, x, 1, 0, n); }
    void assign(int ql, int qr, T x, int i, int l, int r) {
        if (qr <= l || r <= ql) return;
        if (ql <= l && r <= qr) return apply(i, x);
        int m = (l + r) / 2;
        push(i);
        assign(ql, qr, x, 2 * i, l, m), assign(ql, qr, x, 2 * i + 1, m, r);
        pull(i);
    }
    T max(int ql, int qr) { return max(ql, qr, 1, 0, n); }
    T max(int ql, int qr, int i, int l, int r) {
        if (qr <= l || r <= ql) return E;
        if (ql <= l && r <= qr) return v[i];
        int m = (l + r) / 2;
        push(i);
        return std::max(max(ql, qr, 2 * i, l, m), max(ql, qr, 2 * i + 1, m, r));
    }
    T get(int i) { return max(i, i + 1); }
    void set(int i, T x) { assign(i, i + 1, x); }
};
