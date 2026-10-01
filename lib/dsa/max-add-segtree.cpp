// Title: Max-add segment tree
// Description: Range add, range max (ready-made; for other ops use the lazy segment tree).
// Usage:
//   MaxAddSegtree<long long> seg(n);  MaxAddSegtree<long long> seg(n, init);  MaxAddSegtree<long long> seg(a);
//   seg.add(l, r, v)    add v to [l, r)
//   seg.query(l, r)     max of [l, r), requires l < r
//   seg.get(i)
//   For min: store negated values (add -v, negate the answer).
// Complexity: O(log n) per operation.
template <class T> struct MaxAddSegtree {
    int n;
    vector<T> mx, lz;  // lz[i]: pending add for the whole subtree, already included in mx[i]
    MaxAddSegtree(int n_, T init = T{}) : MaxAddSegtree(vector<T>(n_, init)) {}
    MaxAddSegtree(const vector<T> &a) : n((int)a.size()), mx(4 * max(n, 1)), lz(4 * max(n, 1)) {
        if (n) build(1, 0, n, a);
    }
    void build(int i, int l, int r, const vector<T> &a) {
        if (r - l == 1) {
            mx[i] = a[l];
            return;
        }
        int m = (l + r) / 2;
        build(2 * i, l, m, a), build(2 * i + 1, m, r, a);
        mx[i] = max(mx[2 * i], mx[2 * i + 1]);
    }
    void add(int ql, int qr, T v) { add(ql, qr, v, 1, 0, n); }
    void add(int ql, int qr, T v, int i, int l, int r) {
        if (qr <= l || r <= ql) return;
        if (ql <= l && r <= qr) {
            mx[i] += v, lz[i] += v;
            return;
        }
        int m = (l + r) / 2;
        add(ql, qr, v, 2 * i, l, m), add(ql, qr, v, 2 * i + 1, m, r);
        mx[i] = max(mx[2 * i], mx[2 * i + 1]) + lz[i];
    }
    T query(int ql, int qr) { return query(ql, qr, 1, 0, n); }
    T query(int ql, int qr, int i, int l, int r) {
        if (ql <= l && r <= qr) return mx[i];
        int m = (l + r) / 2;
        T res = numeric_limits<T>::lowest();
        if (ql < m) res = query(ql, qr, 2 * i, l, m);
        if (m < qr) res = max(res, query(ql, qr, 2 * i + 1, m, r));
        return res + lz[i];
    }
    T get(int i) { return query(i, i + 1); }
};
