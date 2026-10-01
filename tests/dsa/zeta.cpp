#include "test.h"
#include "dsa/zeta.cpp"
using ll = long long;

int main() {
    for (int it = 0; it < 200; it++) {
        int k = (int)test::rnd(0, 7), n = 1 << k;
        auto a = test::rndVec<ll>(n, -100, 100);
        vector<ll> sub(n, 0), sup(n, 0), subMax(n, LLONG_MIN);
        for (int s = 0; s < n; s++)
            for (int t = 0; t < n; t++) {
                if ((t & s) == t) sub[s] += a[t], subMax[s] = max(subMax[s], a[t]);
                if ((t & s) == s) sup[s] += a[t];
            }
        CHECK_EQ(subsetZeta(a), sub);
        CHECK_EQ(supersetZeta(a), sup);
        CHECK_EQ(subsetMobius(sub), a);
        CHECK_EQ(supersetMobius(sup), a);
        CHECK_EQ(subsetZeta(a, [](ll x, ll y) { return max(x, y); }), subMax);
    }
    // divisor / multiple transforms on indices 1..n (index 0 untouched)
    for (int it = 0; it < 200; it++) {
        int n = (int)test::rnd(0, 120);
        auto a = test::rndVec<ll>(n + 1, -100, 100);
        vector<ll> div(n + 1, 0), mul(n + 1, 0);
        div[0] = mul[0] = a[0];
        for (int m = 1; m <= n; m++)
            for (int d = 1; d <= n; d++)
                if (m % d == 0) div[m] += a[d], mul[d] += a[m];
        CHECK_EQ(divisorZeta(a), div);
        CHECK_EQ(multipleZeta(a), mul);
        CHECK_EQ(divisorMobius(div), a);
        CHECK_EQ(multipleMobius(mul), a);
    }
    // presets: gcd / lcm convolution (indices 1..n, c[0] = 0, lcms above n dropped)
    for (int it = 0; it < 200; it++) {
        int na = (int)test::rnd(0, 80), nb = (int)test::rnd(0, 80);
        const ll mod = 998244353;
        for (ll md : {0LL, mod}) {
            auto a = md ? test::rndVec<ll>(na, 0, md - 1) : test::rndVec<ll>(na, -1000, 1000);
            auto b = md ? test::rndVec<ll>(nb, 0, md - 1) : test::rndVec<ll>(nb, -1000, 1000);
            int n = (int)max(a.size(), b.size());
            vector<ll> g(n, 0), l(n, 0);
            for (int i = 1; i < na; i++)
                for (int j = 1; j < nb; j++) {
                    ll x = md ? (ll)((__int128)a[i] * b[j] % md) : a[i] * b[j];
                    int gc = std::gcd(i, j);
                    ll lc = (ll)i / gc * j;
                    g[gc] = md ? (g[gc] + x) % md : g[gc] + x;
                    if (lc < n) l[lc] = md ? (l[lc] + x) % md : l[lc] + x;
                }
            CHECK_EQ(gcdConv(a, b, md), g);
            CHECK_EQ(lcmConv(a, b, md), l);
        }
    }
    // with a modulus, inputs need not be reduced (negative or huge values)
    for (int it = 0; it < 50; it++) {
        const ll mod = 998244353, big = (ll)1e18;
        int n = (int)test::rnd(1, 60);
        auto a = test::rndVec<ll>(n, -big, big), b = test::rndVec<ll>(n, -big, big);
        vector<ll> ra(n), rb(n);
        for (int i = 0; i < n; i++) ra[i] = (a[i] % mod + mod) % mod, rb[i] = (b[i] % mod + mod) % mod;
        CHECK_EQ(gcdConv(a, b, mod), gcdConv(ra, rb, mod));
        CHECK_EQ(lcmConv(a, b, mod), lcmConv(ra, rb, mod));
        for (ll x : gcdConv(a, b, mod)) CHECK(0 <= x && x < mod);
    }
    CHECK_EQ(gcdConv({}, {}), vector<ll>{});
    CHECK_EQ(gcdConv({7, 1, 2, 3}, {7, 1, 1, 1}), (vector<ll>{0, 13, 2, 3}));
    CHECK_EQ(lcmConv({7, 1, 2, 3}, {7, 1, 1, 1}), (vector<ll>{0, 1, 1 + 2 + 2, 1 + 3 + 3}));
    // doubles work as T
    vector<double> d = {1.5, 2.5};
    CHECK_EQ(subsetZeta(d), (vector<double>{1.5, 4.0}));
}
