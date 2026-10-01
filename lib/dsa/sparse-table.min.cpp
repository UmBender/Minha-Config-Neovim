// Title: Sparse table (min)
// Description: O(1) range minimum on a static array. Plain code, no lambdas.
// Usage:
//   MinSparseTable<long long> st(a);
//   st.query(l, r)   minimum of [l, r), requires l < r
// Complexity: O(n log n) build, O(1) query.
template <class T = long long> struct MinSparseTable {
    vector<vector<T>> t;  // t[k][i] = minimum of [i, i + 2^k)
    MinSparseTable(const vector<T> &a) : t(1, a) {
        int n = (int)a.size();
        for (int k = 1; (1 << k) <= n; k++) {
            t.emplace_back(n - (1 << k) + 1);
            for (int i = 0; i + (1 << k) <= n; i++) t[k][i] = min(t[k - 1][i], t[k - 1][i + (1 << (k - 1))]);
        }
    }
    T query(int l, int r) const {  // two overlapping blocks of length 2^k cover [l, r)
        int k = __lg(r - l);
        return min(t[k][l], t[k][r - (1 << k)]);
    }
};
