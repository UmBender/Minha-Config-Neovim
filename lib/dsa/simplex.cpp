// Title: Simplex (linear programming)
// Description: Maximize c.x subject to A x <= b, x >= 0 (two-phase simplex with Bland-style tie breaking).
// Usage:
//   Simplex lp(A, b, c);            // A: m x n, b: m, c: n (vector<long double>; int literals are fine)
//   vector<long double> x;
//   long double v = lp.solve(x);    // optimum, and x = an optimal point
//   v == -Simplex::INF  infeasible;   v == Simplex::INF  unbounded
//   Constraints >=: negate the row;  equality: add both <= and >=;  minimize: negate c.
// Complexity: exponential worst case, fast in practice (O(m n) per pivot).
// Verify: https://open.kattis.com/problems/roadtimes
// Pending: example + presets (T-009..T-011), remove when done
struct Simplex {
    using ld = long double;
    static constexpr ld EPS = 1e-9, INF = numeric_limits<ld>::infinity();
    int m, n;
    vector<int> N, B;
    vector<vector<ld>> D;
    Simplex(const vector<vector<ld>> &A, const vector<ld> &b, const vector<ld> &c)
        : m((int)b.size()), n((int)c.size()), N(n + 1), B(m), D(m + 2, vector<ld>(n + 2)) {
        for (int i = 0; i < m; i++)
            for (int j = 0; j < n; j++) D[i][j] = A[i][j];
        for (int i = 0; i < m; i++) B[i] = n + i, D[i][n] = -1, D[i][n + 1] = b[i];
        for (int j = 0; j < n; j++) N[j] = j, D[m][j] = -c[j];
        N[n] = -1, D[m + 1][n] = 1;
    }
    void pivot(int r, int s) {
        ld inv = 1 / D[r][s];
        for (int i = 0; i < m + 2; i++)
            if (i != r && fabsl(D[i][s]) > EPS) {
                ld f = D[i][s] * inv;
                for (int j = 0; j < n + 2; j++) D[i][j] -= D[r][j] * f;
                D[i][s] = D[r][s] * f;
            }
        for (int j = 0; j < n + 2; j++)
            if (j != s) D[r][j] *= inv;
        for (int i = 0; i < m + 2; i++)
            if (i != r) D[i][s] *= -inv;
        D[r][s] = inv;
        swap(B[r], N[s]);
    }
    bool simplex(int phase) {
        int x = m + phase - 1;
        while (true) {
            int s = -1;
            for (int j = 0; j <= n; j++)
                if (N[j] != -phase && (s == -1 || make_pair(D[x][j], N[j]) < make_pair(D[x][s], N[s]))) s = j;
            if (D[x][s] >= -EPS) return true;
            int r = -1;
            for (int i = 0; i < m; i++) {
                if (D[i][s] <= EPS) continue;
                if (r == -1 || make_pair(D[i][n + 1] / D[i][s], B[i]) < make_pair(D[r][n + 1] / D[r][s], B[r])) r = i;
            }
            if (r == -1) return false;
            pivot(r, s);
        }
    }
    ld solve(vector<ld> &x) {
        int r = 0;
        for (int i = 1; i < m; i++)
            if (D[i][n + 1] < D[r][n + 1]) r = i;
        if (m > 0 && D[r][n + 1] < -EPS) {
            pivot(r, n);
            if (!simplex(2) || D[m + 1][n + 1] < -EPS) return -INF;
            for (int i = 0; i < m; i++)
                if (B[i] == -1) {
                    int s = 0;
                    for (int j = 1; j <= n; j++)
                        if (make_pair(D[i][j], N[j]) < make_pair(D[i][s], N[s])) s = j;
                    pivot(i, s);
                }
        }
        bool ok = simplex(1);
        x.assign(n, 0);
        for (int i = 0; i < m; i++)
            if (B[i] < n) x[B[i]] = D[i][n + 1];
        return ok ? D[m][n + 1] : INF;
    }
};
