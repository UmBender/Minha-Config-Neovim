#include "test.h"
#include "dsa/zeta.gcd-conv.cpp"
using ll = long long;

int main() {
    // indices 1..n, c[0] = 0
    for (int it = 0; it < 200; it++) {
        int na = (int)test::rnd(0, 80), nb = (int)test::rnd(0, 80);
        const ll mod = 998244353;
        for (ll md : {0LL, mod}) {
            auto a = md ? test::rndVec<ll>(na, 0, md - 1) : test::rndVec<ll>(na, -1000, 1000);
            auto b = md ? test::rndVec<ll>(nb, 0, md - 1) : test::rndVec<ll>(nb, -1000, 1000);
            int n = (int)max(a.size(), b.size());
            vector<ll> g(n, 0);
            for (int i = 1; i < na; i++)
                for (int j = 1; j < nb; j++) {
                    ll x = md ? (ll)((__int128)a[i] * b[j] % md) : a[i] * b[j];
                    int gc = std::gcd(i, j);
                    g[gc] = md ? (g[gc] + x) % md : g[gc] + x;
                }
            CHECK_EQ(gcdConv(a, b, md), g);
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
        for (ll x : gcdConv(a, b, mod)) CHECK(0 <= x && x < mod);
    }
    CHECK_EQ(gcdConv({}, {}), vector<ll>{});
    CHECK_EQ(gcdConv({7, 1, 2, 3}, {7, 1, 1, 1}), (vector<ll>{0, 13, 2, 3}));
}
