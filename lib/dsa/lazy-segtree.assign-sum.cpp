// Title: Lazy segment tree (assign, sum)
// Description: Range assign, range sum. Plain recursive code, no lambdas.
// Usage:
//   RangeAssignSum<long long> seg(n);  RangeAssignSum<long long> seg(a);   // zeros / from a vector
//   seg.assign(l, r, x)   set every element of a[l, r) to x
//   seg.sum(l, r)      sum of [l, r) (0 if empty)
//   seg.set(i, x);  seg.get(i);
// Complexity: O(log n) per operation, O(n) build.
template <class T = long long> struct RangeAssignSum {
    int n;
    vector<T> s, val;  // s[i]: sum of node i; val[i]: assignment still to push to its children
    vector<char> has;  // has[i]: val[i] is pending
    RangeAssignSum(int n_ = 0) : RangeAssignSum(vector<T>(n_, T{})) {}
    RangeAssignSum(const vector<T> &a)
        : n((int)a.size()), s(4 * std::max(n, 1)), val(4 * std::max(n, 1)), has(4 * std::max(n, 1)) {
        if (n) build(1, 0, n, a);
    }
    void build(int i, int l, int r, const vector<T> &a) {
        if (r - l == 1) return void(s[i] = a[l]);
        int m = (l + r) / 2;
        build(2 * i, l, m, a), build(2 * i + 1, m, r, a);
        pull(i);
    }
    void pull(int i) { s[i] = s[2 * i] + s[2 * i + 1]; }
    void apply(int i, int len, T x) { s[i] = x * (T)len, val[i] = x, has[i] = 1; }
    void push(int i, int l, int m, int r) {
        if (has[i]) apply(2 * i, m - l, val[i]), apply(2 * i + 1, r - m, val[i]), has[i] = 0;
    }
    void assign(int ql, int qr, T x) { assign(ql, qr, x, 1, 0, n); }
    void assign(int ql, int qr, T x, int i, int l, int r) {
        if (qr <= l || r <= ql) return;
        if (ql <= l && r <= qr) return apply(i, r - l, x);
        int m = (l + r) / 2;
        push(i, l, m, r);
        assign(ql, qr, x, 2 * i, l, m), assign(ql, qr, x, 2 * i + 1, m, r);
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
    void set(int i, T x) { assign(i, i + 1, x); }
};
