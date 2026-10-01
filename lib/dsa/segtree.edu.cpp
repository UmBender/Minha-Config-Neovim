// Title: Segment tree
// Description: Same code and API as the normal segment tree, commented: how and why it works.
// Usage:
//   Segtree seg(n, e, op);   Segtree seg(a, e, op);       // e = identity, op associative
//   seg.set(i, x);  seg.get(i);  seg.query(l, r) (op over [l, r), e if empty);  seg.all();
//   seg.maxRight(l, pred)  largest r with pred(query(l, r)); pred(e) must be true, pred monotone
//   seg.minLeft(r, pred)   smallest l with pred(query(l, r))
// Complexity: O(log n) per operation, O(n) build.
//
// Idea: a perfect binary tree over the array. Each node stores op() of the range it covers, so
// any range [l, r) splits into O(log n) whole nodes (at most two per level), and changing one
// element only changes the nodes on its path to the root (one per level).
//
// Layout (iterative, "bottom-up"): the tree has sz leaves, sz = smallest power of two >= n.
// Node 1 is the root, the children of node i are 2i and 2i + 1, and leaf j is node sz + j.
// The extra leaves [n, sz) hold the identity e, so they never change an answer.
//
// op only needs to be associative (not commutative): queries keep the left part (sl) and the
// right part (sr) separately and combine them in order at the end.
template <class T, class Op> struct Segtree {
    int n, sz;
    T e;
    Op op;
    vector<T> t;  // t[i] = op over the range of node i (size 2 * sz, t[0] unused)
    Segtree(int n_, T e_, Op op_) : Segtree(vector<T>(n_, e_), e_, op_) {}
    Segtree(const vector<T> &a, T e_, Op op_) : n((int)a.size()), sz(1), e(e_), op(op_) {
        while (sz < n) sz *= 2;
        t.assign(2 * sz, e);
        for (int i = 0; i < n; i++) t[sz + i] = a[i];  // leaves
        for (int i = sz - 1; i >= 1; i--) pull(i);    // parents after children: O(n) build
    }
    // recompute node i from its children
    void pull(int i) { t[i] = op(t[2 * i], t[2 * i + 1]); }
    void set(int i, T x) {
        t[i += sz] = x;
        while (i >>= 1) pull(i);  // walk up: i / 2 is the parent
    }
    T get(int i) const { return t[i + sz]; }
    T all() const { return t[1]; }
    T query(int l, int r) {
        // Move l and r up one level at a time. When l is a right child (odd), its parent also
        // covers elements before l, so take node l on its own and step past it (l++). Same for
        // r (exclusive): when r is odd, node r - 1 is a left child whose parent covers elements
        // from r on, so take it and step back. Each level adds at most one node per side.
        T sl = e, sr = e;
        for (l += sz, r += sz; l < r; l >>= 1, r >>= 1) {
            if (l & 1) sl = op(sl, t[l++]);
            if (r & 1) sr = op(t[--r], sr);
        }
        return op(sl, sr);
    }
    // Binary search on the prefix aggregate: largest r with pred(op over [l, r)) true.
    // Works because pred is monotone (true, ..., true, false, ...).
    template <class P> int maxRight(int l, P pred) {
        if (l == n) return n;
        l += sz;
        T s = e;  // op over [original l, start of node l)
        do {
            // climb while l is a left child: its parent starts at the same position and is bigger
            while (l % 2 == 0) l >>= 1;
            if (!pred(op(s, t[l]))) {
                // the answer is inside node l: descend, taking the left child whenever it fits
                while (l < sz) {
                    l = 2 * l;
                    if (pred(op(s, t[l]))) s = op(s, t[l++]);
                }
                return l - sz;
            }
            s = op(s, t[l++]);  // the whole node fits: take it and move to the next node
        } while ((l & -l) != l);  // l a power of two: we reached the right end of the tree
        return n;
    }
    // Mirror of maxRight: smallest l with pred(op over [l, r)) true.
    template <class P> int minLeft(int r, P pred) {
        if (r == 0) return 0;
        r += sz;
        T s = e;  // op over [end of node r, original r)
        do {
            r--;
            while (r > 1 && r % 2) r >>= 1;  // climb while r is a right child
            if (!pred(op(t[r], s))) {
                while (r < sz) {
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
