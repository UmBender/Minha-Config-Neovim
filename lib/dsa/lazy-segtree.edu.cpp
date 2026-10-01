// Title: Lazy segment tree
// Description: Same code and API as the normal lazy segment tree, commented: how and why it works.
// Usage:
//   LazySegtree seg(n or a, e, id, op, mapping, compose);
//     T: node value, e: identity of op;  U: update, id: identity update
//     op(T, T) -> T            combine (associative)
//     mapping(U f, T x) -> T   apply f to a node (store the length in T if f depends on it)
//     compose(U f, U g) -> U   f after g (g was applied first)
//   seg.set(i, x); seg.get(i); seg.query(l, r); seg.all(); seg.apply(i, f); seg.apply(l, r, f);
//   seg.maxRight(l, pred) / seg.minLeft(r, pred)   as in Segtree
// Complexity: O(log n) per operation.
//
// Idea: a segment tree (see the educational Segtree for the layout: root 1, children 2i and
// 2i + 1, leaf j at sz + j) where a range update is not pushed down to every element. Like a
// range query, [l, r) splits into O(log n) whole nodes; each of them gets the update applied to
// its value right away (mapping) and remembers it in lz[i] as "still owed to my children".
// Whenever we need to look inside a node (go below it), we first push its pending update to
// its two children (push). So every operation touches O(log n) nodes.
//
// Requirements, so that a node's value can be updated without looking at its elements:
//   mapping(f, op(x, y)) == op(mapping(f, x), mapping(f, y))   (f distributes over op)
//   mapping(compose(f, g), x) == mapping(f, mapping(g, x))     (pending updates can be merged)
// e.g. "add f to every element" on sums needs the length: mapping(f, {sum, len}) = {sum + f len, len}.
template <class T, class U, class Op, class Map, class Comp> struct LazySegtree {
    int n, sz, lg;
    T e;
    U id;
    Op op;
    Map mapping;
    Comp compose;
    vector<T> t;   // t[i]: op over node i's range, with every update applied so far
    vector<U> lz;  // lz[i]: update applied to node i but not yet to its children (inner nodes only)
    LazySegtree(int n_, T e_, U id_, Op op_, Map map_, Comp comp_)
        : LazySegtree(vector<T>(n_, e_), e_, id_, op_, map_, comp_) {}
    LazySegtree(const vector<T> &a, T e_, U id_, Op op_, Map map_, Comp comp_)
        : n((int)a.size()), sz(1), lg(0), e(e_), id(id_), op(op_), mapping(map_), compose(comp_) {
        while (sz < n) sz *= 2, lg++;  // sz = 2^lg leaves
        t.assign(2 * sz, e);
        lz.assign(sz, id);
        for (int i = 0; i < n; i++) t[sz + i] = a[i];
        for (int i = sz - 1; i >= 1; i--) pull(i);
    }
    // recompute node i from its children (only valid when lz[i] is the identity, i.e. after a push)
    void pull(int i) { t[i] = op(t[2 * i], t[2 * i + 1]); }
    // apply f to the whole node i: update its value, and owe f to its children
    void applyNode(int i, U f) {
        t[i] = mapping(f, t[i]);
        if (i < sz) lz[i] = compose(f, lz[i]);
    }
    // hand the pending update of node i to its children
    void push(int i) {
        applyNode(2 * i, lz[i]);
        applyNode(2 * i + 1, lz[i]);
        lz[i] = id;
    }
    // push every ancestor of leaf/node p, top-down, so that p's value is exact
    void pushPath(int p) {
        for (int i = lg; i >= 1; i--) push(p >> i);
    }
    // recompute every ancestor of p, bottom-up, after p changed
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
        // The nodes we read are below the ancestors of the leaves l and r - 1, so those
        // ancestors must push first. Ancestors whose range starts exactly at l (or ends at r) are
        // read as whole nodes or not at all, so they can be skipped (the shift test below).
        for (int i = lg; i >= 1; i--) {
            if (((l >> i) << i) != l) push(l >> i);
            if (((r >> i) << i) != r) push((r - 1) >> i);
        }
        T sl = e, sr = e;  // same walk as Segtree::query
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
        // apply f to the O(log n) nodes that cover [l, r) exactly (same walk as a query)
        for (int a = l, b = r; a < b; a >>= 1, b >>= 1) {
            if (a & 1) applyNode(a++, f);
            if (b & 1) applyNode(--b, f);
        }
        // their ancestors now have stale values: recompute them bottom-up
        for (int i = 1; i <= lg; i++) {
            if (((l >> i) << i) != l) pull(l >> i);
            if (((r >> i) << i) != r) pull((r - 1) >> i);
        }
    }
    // Same binary searches as Segtree, pushing every node before descending into it.
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

