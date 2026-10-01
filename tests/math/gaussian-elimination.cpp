#include "test.h"
#include "math/powm.cpp"
#include "math/gaussian-elimination.cpp"
using ll = long long;
using Mat = vector<vector<ll>>;

Mat rndMat(int n, int m, ll lo, ll hi) {
    Mat a(n);
    for (auto &r : a) r = test::rndVec<ll>(m, lo, hi);
    return a;
}

// all vectors in the row space of a (mod p), by enumeration
set<vector<ll>> rowSpan(const Mat &a, int m, ll p) {
    set<vector<ll>> s;
    int n = (int)a.size();
    vector<ll> coef(n, 0);
    for (;;) {
        vector<ll> v(m, 0);
        for (int i = 0; i < n; i++)
            for (int j = 0; j < m; j++) v[j] = ((v[j] + coef[i] * a[i][j]) % p + p) % p;
        s.insert(v);
        int i = 0;
        while (i < n && coef[i] == p - 1) coef[i++] = 0;
        if (i == n) break;
        coef[i]++;
    }
    return s;
}

void checkRref(const Mat &a, int rank, ll p) {
    int last = -1;
    for (int i = 0; i < (int)a.size(); i++) {
        int lead = 0, m = (int)a[i].size();
        for (ll x : a[i]) CHECK(0 <= x && x < p);
        while (lead < m && a[i][lead] == 0) lead++;
        if (i >= rank) {
            CHECK_EQ(lead, m); // zero rows at the bottom
            continue;
        }
        CHECK(lead < m && lead > last);
        CHECK_EQ(a[i][lead], 1LL);
        for (int k = 0; k < (int)a.size(); k++)
            if (k != i) CHECK_EQ(a[k][lead], 0LL);
        last = lead;
    }
}

ll naiveDet(const Mat &a, ll p) {
    int n = (int)a.size();
    vector<int> perm(n);
    iota(perm.begin(), perm.end(), 0);
    ll det = 0;
    do {
        ll t = 1;
        for (int i = 0; i < n; i++) t = t * ((a[i][perm[i]] % p + p) % p) % p;
        int inv = 0;
        for (int i = 0; i < n; i++)
            for (int j = i + 1; j < n; j++) inv += perm[i] > perm[j];
        det = (det + (inv % 2 ? p - t : t)) % p;
    } while (next_permutation(perm.begin(), perm.end()));
    return det;
}

int main() {
    {
        Mat e;
        CHECK_EQ(gaussMod(e), 0);
        CHECK_EQ(detMod(e), 1LL);
        Mat a = {{2, 4, 6}, {1, 2, 4}};
        CHECK_EQ(gaussMod(a, 7), 2);
        CHECK_EQ(a, (Mat{{1, 2, 0}, {0, 0, 1}}));
        Mat z = {{0, 0}, {0, 0}};
        CHECK_EQ(gaussMod(z), 0);
        CHECK_EQ(detMod({{1, 2}, {3, 4}}), 998244351LL);
        CHECK_EQ(detMod({{0, 1}, {1, 0}}, 7), 6LL);
        CHECK_EQ(detMod({{-1}}, 7), 6LL);
    }
    // rank and row space against enumeration, mod 3 and mod 2
    for (ll p : {2LL, 3LL}) {
        for (int it = 0; it < 600; it++) {
            int n = (int)test::rnd(1, 4), m = (int)test::rnd(1, 5);
            Mat a = rndMat(n, m, -5, 5), b = a;
            int r = gaussMod(b, p);
            auto s = rowSpan(a, m, p);
            CHECK_EQ((ll)s.size(), (ll)round(pow(p, r)));
            CHECK(rowSpan(b, m, p) == s);
            checkRref(b, r, p);
        }
    }
    // large prime: RREF shape, and row space via rank of [a; b]
    for (int it = 0; it < 300; it++) {
        int n = (int)test::rnd(1, 8), m = (int)test::rnd(1, 8);
        Mat a = rndMat(n, m, 0, it % 3 ? 998244352 : 2); // small entries: many dependencies
        if (it % 4 == 0) a.push_back(a[0]);              // a dependent row
        Mat b = a;
        int r = gaussMod(b);
        checkRref(b, r, 998244353);
        CHECK(r <= min((int)a.size(), m));
        Mat both = a;
        for (int i = 0; i < r; i++) both.push_back(b[i]);
        CHECK_EQ(gaussMod(both), r);
    }
    // determinant against the permutation expansion
    for (ll p : {2LL, 7LL, 998244353LL, 1000000007LL})
        for (int it = 0; it < 200; it++) {
            int n = (int)test::rnd(1, 6);
            Mat a = rndMat(n, n, -20, p == 2 ? 1 : 20);
            CHECK_EQ(detMod(a, p), naiveDet(a, p));
        }
    // solve: solutions satisfy the system; "no solution" checked by enumeration mod 3
    for (int it = 0; it < 600; it++) {
        int n = (int)test::rnd(1, 4), m = (int)test::rnd(1, 4);
        ll p = it % 2 ? 3 : 998244353;
        Mat a = rndMat(n, m, 0, min(p - 1, 3LL));
        vector<ll> b = test::rndVec<ll>(n, 0, min(p - 1, 3LL));
        if (it % 3 == 0) { // consistent by construction
            auto x0 = test::rndVec<ll>(m, 0, p - 1);
            for (int i = 0; i < n; i++) {
                b[i] = 0;
                for (int j = 0; j < m; j++) b[i] = (b[i] + a[i][j] * x0[j]) % p;
            }
        }
        auto x = solveMod(a, b, p);
        if (x) {
            CHECK_EQ((int)x->size(), m);
            for (int i = 0; i < n; i++) {
                ll v = 0;
                for (int j = 0; j < m; j++) v = (v + a[i][j] * (*x)[j]) % p;
                CHECK_EQ(v, b[i]);
            }
        } else {
            CHECK(it % 3 != 0);
            if (p == 3) {
                bool any = false;
                for (int mask = 0; mask < 81 && !any; mask++) {
                    vector<ll> y(m);
                    for (int j = 0, t = mask; j < m; j++, t /= 3) y[j] = t % 3;
                    bool ok = true;
                    for (int i = 0; i < n; i++) {
                        ll v = 0;
                        for (int j = 0; j < m; j++) v += a[i][j] * y[j];
                        ok &= v % 3 == b[i];
                    }
                    any |= ok;
                }
                CHECK(!any);
            }
        }
    }
    // a 300x300 system (cubic) with a known solution
    {
        int n = 300;
        Mat a = rndMat(n, n, 0, 998244352);
        auto x0 = test::rndVec<ll>(n, 0, 998244352);
        vector<ll> b(n, 0);
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++) b[i] = (b[i] + a[i][j] * x0[j]) % 998244353;
        CHECK_EQ(*solveMod(a, b), x0); // a random matrix is invertible w.h.p.
    }
}
