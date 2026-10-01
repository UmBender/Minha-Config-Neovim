#include "test.h"
#include "dsa/simplex.cpp"
using ld = long double;

// maximize c.x subject to A x <= b, x >= 0 by enumerating vertices (small n)
ld brute(const vector<vector<ld>> &A, const vector<ld> &b, const vector<ld> &c, bool &feasible) {
    int m = (int)A.size(), n = (int)c.size();
    vector<vector<ld>> rows = A;
    vector<ld> rhs = b;
    for (int j = 0; j < n; j++) {
        vector<ld> r(n, 0);
        r[j] = -1;
        rows.push_back(r), rhs.push_back(0);
    }
    int total = m + n;
    ld best = -numeric_limits<ld>::infinity();
    feasible = false;
    for (int mask = 0; mask < (1 << total); mask++) {
        if (__builtin_popcount(mask) != n) continue;
        vector<vector<ld>> M;
        for (int i = 0; i < total; i++)
            if (mask >> i & 1) {
                auto r = rows[i];
                r.push_back(rhs[i]);
                M.push_back(r);
            }
        bool singular = false;
        for (int col = 0; col < n && !singular; col++) {
            int piv = col;
            for (int i = col; i < n; i++)
                if (fabsl(M[i][col]) > fabsl(M[piv][col])) piv = i;
            if (fabsl(M[piv][col]) < 1e-9) { singular = true; break; }
            swap(M[col], M[piv]);
            for (int i = 0; i < n; i++)
                if (i != col) {
                    ld f = M[i][col] / M[col][col];
                    for (int k = col; k <= n; k++) M[i][k] -= f * M[col][k];
                }
        }
        if (singular) continue;
        vector<ld> x(n);
        for (int i = 0; i < n; i++) x[i] = M[i][n] / M[i][i];
        bool ok = true;
        for (int i = 0; i < total && ok; i++) {
            ld s = 0;
            for (int j = 0; j < n; j++) s += rows[i][j] * x[j];
            if (s > rhs[i] + 1e-7) ok = false;
        }
        if (!ok) continue;
        feasible = true;
        ld v = 0;
        for (int j = 0; j < n; j++) v += c[j] * x[j];
        best = max(best, v);
    }
    return best;
}

int main() {
    // classic: max 3x + 2y, x + y <= 4, x + 3y <= 6, x <= 3  ->  x = 3, y = 1, value 11
    {
        vector<ld> x;
        Simplex lp({{1, 1}, {1, 3}, {1, 0}}, {4, 6, 3}, {3, 2});
        CHECK_NEAR(lp.solve(x), 11, 1e-9);
        CHECK_NEAR(x[0], 3, 1e-9);
        CHECK_NEAR(x[1], 1, 1e-9);
    }
    {  // infeasible: x <= -1 with x >= 0
        vector<ld> x;
        CHECK(Simplex({{1}}, {-1}, {1}).solve(x) == -Simplex::INF);
    }
    {  // unbounded: max x with -x <= 1
        vector<ld> x;
        CHECK(Simplex({{-1}}, {1}, {1}).solve(x) == Simplex::INF);
    }
    // random bounded LPs (box x_j <= 10) vs vertex enumeration
    for (int it = 0; it < 400; it++) {
        int n = (int)test::rnd(1, 3), m = (int)test::rnd(0, 4);
        vector<vector<ld>> A;
        vector<ld> b, c(n);
        for (int i = 0; i < m; i++) {
            vector<ld> row(n);
            for (auto &v : row) v = (ld)test::rnd(-5, 5);
            A.push_back(row), b.push_back((ld)test::rnd(-5, 20));
        }
        for (int j = 0; j < n; j++) {
            vector<ld> row(n, 0);
            row[j] = 1;
            A.push_back(row), b.push_back(10);
        }
        for (auto &v : c) v = (ld)test::rnd(-5, 5);
        bool feasible;
        ld want = brute(A, b, c, feasible);
        vector<ld> x;
        ld got = Simplex(A, b, c).solve(x);
        if (!feasible) {
            CHECK(got == -Simplex::INF);
            continue;
        }
        CHECK_NEAR(got, want, 1e-6);
        // the returned point is feasible and achieves the value
        ld val = 0;
        for (int j = 0; j < n; j++) CHECK(x[j] >= -1e-7), val += c[j] * x[j];
        CHECK_NEAR(val, got, 1e-6);
        for (size_t i = 0; i < A.size(); i++) {
            ld s = 0;
            for (int j = 0; j < n; j++) s += A[i][j] * x[j];
            CHECK(s <= b[i] + 1e-6);
        }
    }
}
