// Title: Gaussian elimination
// Description: Row reduction modulo a prime: rank and reduced row echelon form, determinant, solving A x = b.
// Usage:
//   int r = gaussMod(a, mod);            // a: n x m, reduced in place to RREF (entries in [0, mod)); returns the rank
//   detMod(a, mod)                       // determinant of an n x n matrix (a is copied)
//   auto x = solveMod(A, b, mod);        // optional<vector<long long>>: a solution (free variables 0), nullopt if none
//   // prime mod < 2^31 (998244353 by default), entries of any sign
// Complexity: O(n m min(n, m)); detMod O(n^3).
// Verify: https://judge.yosupo.jp/problem/matrix_rank
// Verify: https://judge.yosupo.jp/problem/matrix_det
// Requires: math/powm
int gaussMod(vector<vector<long long>> &a, long long mod = 998244353) {
    int n = (int)a.size(), m = n ? (int)a[0].size() : 0, r = 0;
    for (auto &row : a)
        for (auto &x : row) x = (x % mod + mod) % mod;
    for (int c = 0; c < m && r < n; c++) {
        int p = r;
        while (p < n && !a[p][c]) p++;
        if (p == n) continue;
        swap(a[p], a[r]);
        long long inv = powMod(a[r][c], mod - 2, mod);
        for (int j = c; j < m; j++) a[r][j] = a[r][j] * inv % mod;
        for (int i = 0; i < n; i++)
            if (i != r && a[i][c]) {
                long long f = a[i][c];
                for (int j = c; j < m; j++) a[i][j] = (a[i][j] - f * a[r][j] % mod + mod) % mod;
            }
        r++;
    }
    return r;
}

long long detMod(vector<vector<long long>> a, long long mod = 998244353) {
    int n = (int)a.size();
    long long det = 1 % mod;
    for (auto &row : a)
        for (auto &x : row) x = (x % mod + mod) % mod;
    for (int c = 0; c < n; c++) {
        int p = c;
        while (p < n && !a[p][c]) p++;
        if (p == n) return 0;
        if (p != c) swap(a[p], a[c]), det = (mod - det) % mod;
        det = det * a[c][c] % mod;
        long long inv = powMod(a[c][c], mod - 2, mod);
        for (int i = c + 1; i < n; i++)
            if (long long f = a[i][c] * inv % mod)
                for (int j = c; j < n; j++) a[i][j] = (a[i][j] - f * a[c][j] % mod + mod) % mod;
    }
    return det;
}

optional<vector<long long>> solveMod(vector<vector<long long>> a, const vector<long long> &b,
                                     long long mod = 998244353) {
    int n = (int)a.size(), m = n ? (int)a[0].size() : 0;
    for (int i = 0; i < n; i++) a[i].push_back(b[i]);
    int r = gaussMod(a, mod);
    vector<long long> x(m, 0);
    for (int i = 0; i < r; i++) {
        int lead = 0;
        while (!a[i][lead]) lead++;
        if (lead == m) return nullopt;  // 0 = nonzero
        x[lead] = a[i][m];
    }
    return x;
}
