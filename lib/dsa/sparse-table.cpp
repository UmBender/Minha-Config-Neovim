// Title: Sparse table
// Description: O(1) range queries for idempotent ops (min, max, gcd, and, or) on a static array.
// Usage:
//   SparseTable st(a, [](int x, int y) { return min(x, y); });
//   st.query(l, r)   op over [l, r), requires l < r
//   op must be idempotent (op(x, x) == x): sums need a prefix-sum array instead.
//   Plain min / max / gcd tables: the min, max, gcd variants in the <leader>rl menu.
// Complexity: O(n log n) build, O(1) query.
// Verify: https://judge.yosupo.jp/problem/staticrmq
template <class T, class Op> struct SparseTable {
    Op op;
    vector<vector<T>> t;
    SparseTable(const vector<T> &a, Op op_) : op(op_), t(1, a) {
        int n = (int)a.size();
        for (int k = 1; (1 << k) <= n; k++) {
            t.emplace_back(n - (1 << k) + 1);
            for (int i = 0; i + (1 << k) <= n; i++) t[k][i] = op(t[k - 1][i], t[k - 1][i + (1 << (k - 1))]);
        }
    }
    T query(int l, int r) {
        int k = __lg(r - l);
        return op(t[k][l], t[k][r - (1 << k)]);
    }
};

