// Title: Lazy segment tree (add, sum)
// Description: Range add, range sum. Plain recursive code, no lambdas.
// Usage:
//   RangeAddSum<long long> seg(n);  RangeAddSum<long long> seg(a);   // zeros / from a vector
//   seg.add(l, r, x)   add x to a[l, r)
//   seg.sum(l, r)      sum of [l, r) (0 if empty)
//   seg.set(i, x);  seg.get(i);
// Complexity: O(log n) per operation, O(n) build.
template <class T = long long> struct RangeAddSum {
    int n;
    vector<T> s, lz;  // s[i]: sum of node i, lz[i]: add still to push to its children
    RangeAddSum(int n_ = 0) : RangeAddSum(vector<T>(n_, T{})) {}
    RangeAddSum(const vector<T> &a) : n((int)a.size()), s(4 * std::max(n, 1)), lz(4 * std::max(n, 1)) {
        if (n) build(1, 0, n, a);
    }
    void build(int i, int l, int r, const vector<T> &a) {
        if (r - l == 1) return void(s[i] = a[l]);
        int m = (l + r) / 2;
        build(2 * i, l, m, a), build(2 * i + 1, m, r, a);
        pull(i);
    }
    void pull(int i) { s[i] = s[2 * i] + s[2 * i + 1]; }
    void apply(int i, int len, T x) { s[i] += x * (T)len, lz[i] += x; }
    void push(int i, int l, int m, int r) {
        apply(2 * i, m - l, lz[i]), apply(2 * i + 1, r - m, lz[i]), lz[i] = T{};
    }
    void add(int ql, int qr, T x) { add(ql, qr, x, 1, 0, n); }
    void add(int ql, int qr, T x, int i, int l, int r) {
        if (qr <= l || r <= ql) return;
        if (ql <= l && r <= qr) return apply(i, r - l, x);
        int m = (l + r) / 2;
        push(i, l, m, r);
        add(ql, qr, x, 2 * i, l, m), add(ql, qr, x, 2 * i + 1, m, r);
        pull(i);
    }
    T sum(int ql, int qr) { return sum(ql, qr, 1, 0, n); }
    T sum(int ql, int qr, int i, int l, int r) {
        if (qr <= l || r <= ql) return T{};
        if (ql <= l && r <= qr) return s[i];
        int m = (l + r) / 2;
        push(i, l, m, r);
        return sum(ql, qr, 2 * i, l, m) + sum(ql, qr, 2 * i + 1, m, r);
    }
    T get(int i) { return sum(i, i + 1); }
    void set(int i, T x) { add(i, i + 1, x - get(i)); }
};
