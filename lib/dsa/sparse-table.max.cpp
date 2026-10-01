// Title: Sparse table (max)
// Description: O(1) range maximum on a static array. Plain code, no lambdas.
// Usage:
//   MaxSparseTable<long long> st(a);
//   st.query(l, r)   maximum of [l, r), requires l < r
// Complexity: O(n log n) build, O(1) query.
template <class T = long long> struct MaxSparseTable {
    vector<vector<T>> t;  // t[k][i] = maximum of [i, i + 2^k)
    MaxSparseTable(const vector<T> &a) : t(1, a) {
        int n = (int)a.size();
        for (int k = 1; (1 << k) <= n; k++) {
            t.emplace_back(n - (1 << k) + 1);
            for (int i = 0; i + (1 << k) <= n; i++) t[k][i] = max(t[k - 1][i], t[k - 1][i + (1 << (k - 1))]);
        }
    }
    T query(int l, int r) const {  // two overlapping blocks of length 2^k cover [l, r)
        int k = __lg(r - l);
        return max(t[k][l], t[k][r - (1 << k)]);
    }
};
