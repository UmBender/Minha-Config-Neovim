// Title: Sparse table
// Description: Same code and API as the normal sparse table, commented: how and why it works.
// Usage:
//   SparseTable st(a, [](int x, int y) { return min(x, y); });
//   st.query(l, r)   op over [l, r), requires l < r
//   op must be idempotent (op(x, x) == x): sums need a prefix-sum array instead.
//   Plain min / max / gcd tables: the min, max, gcd variants in the <leader>rl menu.
// Complexity: O(n log n) build, O(1) query.
//
// Idea: precompute op over every block whose length is a power of two: t[k][i] = op over
// [i, i + 2^k), built from two halves of length 2^(k - 1) (n log n values). Any range [l, r) is
// covered by two such blocks of length 2^k, k = floor(log2(r - l)): [l, l + 2^k) and
// [r - 2^k, r). They overlap, so this only works when counting an element twice doesn't matter
// (idempotent op: min, max, gcd, and, or), and the query is O(1).
template <class T, class Op> struct SparseTable {
    Op op;
    vector<vector<T>> t;  // t[k][i] = op over [i, i + 2^k)
    SparseTable(const vector<T> &a, Op op_) : op(op_), t(1, a) {
        int n = (int)a.size();
        for (int k = 1; (1 << k) <= n; k++) {
            t.emplace_back(n - (1 << k) + 1);
            for (int i = 0; i + (1 << k) <= n; i++) t[k][i] = op(t[k - 1][i], t[k - 1][i + (1 << (k - 1))]);
        }
    }
    T query(int l, int r) {
        int k = __lg(r - l);  // largest k with 2^k <= r - l (__lg: index of the highest set bit)
        return op(t[k][l], t[k][r - (1 << k)]);
    }
};

