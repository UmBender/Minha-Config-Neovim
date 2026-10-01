// Title: Min-add segment tree
// Description: Range add, range min (ready-made; for other ops use the lazy segment tree).
// Usage:
//   MinAddSegtree<long long> seg(n);  MinAddSegtree<long long> seg(n, init);  MinAddSegtree<long long> seg(a);
//   seg.add(l, r, v)    add v to [l, r)
//   seg.query(l, r)     min of [l, r), requires l < r
//   seg.get(i)
//   T defaults to long long: MinAddSegtree seg(n).
// Complexity: O(log n) per operation.
template <class T = long long> struct MinAddSegtree {
    int n;
    vector<T> mn, lz;  // lz[i]: pending add for the whole subtree, already included in mn[i]
    MinAddSegtree(int n_, T init = T{}) : MinAddSegtree(vector<T>(n_, init)) {}
    MinAddSegtree(const vector<T> &a) : n((int)a.size()), mn(4 * max(n, 1)), lz(4 * max(n, 1)) {
        if (n) build(1, 0, n, a);
    }
    void build(int i, int l, int r, const vector<T> &a) {
        if (r - l == 1) {
            mn[i] = a[l];
            return;
        }
        int m = (l + r) / 2;
        build(2 * i, l, m, a), build(2 * i + 1, m, r, a);
        mn[i] = min(mn[2 * i], mn[2 * i + 1]);
    }
    void add(int ql, int qr, T v) { add(ql, qr, v, 1, 0, n); }
    void add(int ql, int qr, T v, int i, int l, int r) {
        if (qr <= l || r <= ql) return;
        if (ql <= l && r <= qr) {
            mn[i] += v, lz[i] += v;
            return;
        }
        int m = (l + r) / 2;
        add(ql, qr, v, 2 * i, l, m), add(ql, qr, v, 2 * i + 1, m, r);
        mn[i] = min(mn[2 * i], mn[2 * i + 1]) + lz[i];
    }
    T query(int ql, int qr) { return query(ql, qr, 1, 0, n); }
    T query(int ql, int qr, int i, int l, int r) {
        if (ql <= l && r <= qr) return mn[i];
        int m = (l + r) / 2;
        T res = numeric_limits<T>::max();
        if (ql < m) res = query(ql, qr, 2 * i, l, m);
        if (m < qr) res = min(res, query(ql, qr, 2 * i + 1, m, r));
        return res + lz[i];
    }
    T get(int i) { return query(i, i + 1); }
};

