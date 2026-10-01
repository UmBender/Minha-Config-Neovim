// Title: Lazy segment tree
// Description: Generic lazy segment tree (range update, range query, binary search), ops as lambdas.
// Usage:
//   LazySegtree seg(n or a, e, id, op, mapping, compose);
//     T: node value, e: identity of op;  U: update, id: identity update
//     op(T, T) -> T            combine (associative)
//     mapping(U f, T x) -> T   apply f to a node (store the length in T if f depends on it)
//     compose(U f, U g) -> U   f after g (g was applied first)
//   seg.set(i, x); seg.get(i); seg.query(l, r); seg.all(); seg.apply(i, f); seg.apply(l, r, f);
//   seg.maxRight(l, pred) / seg.minLeft(r, pred)   as in Segtree
//   Range add + range sum:
//     struct Node { long long sum, len; };   // init each leaf as {a[i], 1}
//     LazySegtree seg(init, Node{0, 0}, 0LL,
//         [](Node x, Node y) { return Node{x.sum + y.sum, x.len + y.len}; },
//         [](long long f, Node x) { return Node{x.sum + f * x.len, x.len}; },
//         [](long long f, long long g) { return f + g; });
//   Range add + range min: T = long long, e = LLONG_MAX, mapping = x == e ? x : x + f
// Complexity: O(log n) per operation.
// Verify: https://judge.yosupo.jp/problem/range_affine_range_sum
template <class T, class U, class Op, class Map, class Comp> struct LazySegtree {
    int n, sz, lg;
    T e;
    U id;
    Op op;
    Map mapping;
    Comp compose;
    vector<T> t;
    vector<U> lz;
    LazySegtree(int n_, T e_, U id_, Op op_, Map map_, Comp comp_)
        : LazySegtree(vector<T>(n_, e_), e_, id_, op_, map_, comp_) {}
    LazySegtree(const vector<T> &a, T e_, U id_, Op op_, Map map_, Comp comp_)
        : n((int)a.size()), sz(1), lg(0), e(e_), id(id_), op(op_), mapping(map_), compose(comp_) {
        while (sz < n) sz *= 2, lg++;
        t.assign(2 * sz, e);
        lz.assign(sz, id);
        for (int i = 0; i < n; i++) t[sz + i] = a[i];
        for (int i = sz - 1; i >= 1; i--) pull(i);
    }
    void pull(int i) { t[i] = op(t[2 * i], t[2 * i + 1]); }
    void applyNode(int i, U f) {
        t[i] = mapping(f, t[i]);
        if (i < sz) lz[i] = compose(f, lz[i]);
    }
    void push(int i) {
        applyNode(2 * i, lz[i]);
        applyNode(2 * i + 1, lz[i]);
        lz[i] = id;
    }
    void pushPath(int p) {
        for (int i = lg; i >= 1; i--) push(p >> i);
    }
    void pullPath(int p) {
        for (int i = 1; i <= lg; i++) pull(p >> i);
    }
    void set(int p, T x) {
        p += sz;
        pushPath(p);
        t[p] = x;
        pullPath(p);
    }
    T get(int p) {
        p += sz;
        pushPath(p);
        return t[p];
    }
    T all() const { return t[1]; }
    T query(int l, int r) {
        if (l == r) return e;
        l += sz, r += sz;
        for (int i = lg; i >= 1; i--) {
            if (((l >> i) << i) != l) push(l >> i);
            if (((r >> i) << i) != r) push((r - 1) >> i);
        }
        T sl = e, sr = e;
        for (; l < r; l >>= 1, r >>= 1) {
            if (l & 1) sl = op(sl, t[l++]);
            if (r & 1) sr = op(t[--r], sr);
        }
        return op(sl, sr);
    }
    void apply(int p, U f) {
        p += sz;
        pushPath(p);
        t[p] = mapping(f, t[p]);
        pullPath(p);
    }
    void apply(int l, int r, U f) {
        if (l == r) return;
        l += sz, r += sz;
        for (int i = lg; i >= 1; i--) {
            if (((l >> i) << i) != l) push(l >> i);
            if (((r >> i) << i) != r) push((r - 1) >> i);
        }
        for (int a = l, b = r; a < b; a >>= 1, b >>= 1) {
            if (a & 1) applyNode(a++, f);
            if (b & 1) applyNode(--b, f);
        }
        for (int i = 1; i <= lg; i++) {
            if (((l >> i) << i) != l) pull(l >> i);
            if (((r >> i) << i) != r) pull((r - 1) >> i);
        }
    }
    template <class P> int maxRight(int l, P pred) {
        if (l == n) return n;
        l += sz;
        pushPath(l);
        T s = e;
        do {
            while (l % 2 == 0) l >>= 1;
            if (!pred(op(s, t[l]))) {
                while (l < sz) {
                    push(l);
                    l = 2 * l;
                    if (pred(op(s, t[l]))) s = op(s, t[l++]);
                }
                return l - sz;
            }
            s = op(s, t[l++]);
        } while ((l & -l) != l);
        return n;
    }
    template <class P> int minLeft(int r, P pred) {
        if (r == 0) return 0;
        r += sz;
        pushPath(r - 1);
        T s = e;
        do {
            r--;
            while (r > 1 && r % 2) r >>= 1;
            if (!pred(op(t[r], s))) {
                while (r < sz) {
                    push(r);
                    r = 2 * r + 1;
                    if (pred(op(t[r], s))) s = op(t[r--], s);
                }
                return r + 1 - sz;
            }
            s = op(t[r], s);
        } while ((r & -r) != r);
        return 0;
    }
};
