#include "test.h"
#include "math/egcd.cpp"
#include "math/crt.cpp"
using ll = long long;

// smallest x >= 0 with x = r1 (m1), x = r2 (m2), or -1
ll naive(ll r1, ll m1, ll r2, ll m2) {
    ll l = lcm(m1, m2);
    for (ll x = 0; x < l; x++)
        if (x % m1 == r1 && x % m2 == r2) return x;
    return -1;
}

int main() {
    CHECK_EQ(crt(2, 3, 3, 5), (pair<ll, ll>{8, 15}));
    CHECK_EQ(crt(1, 4, 2, 6), (pair<ll, ll>{0, -1}));
    CHECK_EQ(crt(1, 4, 3, 6), (pair<ll, ll>{9, 12}));
    CHECK_EQ(crt(0, 1, 0, 1), (pair<ll, ll>{0, 1}));
    CHECK_EQ(crt(-1, 5, 7, 3), (pair<ll, ll>{4, 15})); // residues are normalized
    for (int it = 0; it < 4000; it++) {
        ll m1 = test::rnd(1, 40), m2 = test::rnd(1, 40);
        ll r1 = test::rnd(0, m1 - 1), r2 = test::rnd(0, m2 - 1);
        ll want = naive(r1, m1, r2, m2);
        auto [r, m] = crt(r1, m1, r2, m2);
        if (want < 0) CHECK_EQ(m, -1LL);
        else CHECK_EQ((pair<ll, ll>{r, m}), (pair<ll, ll>{want, lcm(m1, m2)}));
    }
    // large moduli: lcm up to ~1e18 (products need __int128)
    for (int it = 0; it < 4000; it++) {
        ll g = test::rnd(1, it % 2 ? 1 : 1000);
        ll a = test::rnd(1, 1000000000LL / g), b = test::rnd(1, 1000000000LL / g);
        ll m1 = g * a, m2 = g * b;
        ll x = test::rnd(0, lcm(m1, m2) - 1);
        auto [r, m] = crt(x % m1, m1, x % m2, m2);
        CHECK_EQ((pair<ll, ll>{r, m}), (pair<ll, ll>{x, lcm(m1, m2)}));
    }
    // one small and one huge modulus: m2 / g > 2^32, so q * x overflows without __int128
    for (int it = 0; it < 4000; it++) {
        ll m1 = test::rnd(1, 1000), m2 = test::rnd(1, 1000000000000000LL);
        ll x = test::rnd(0, lcm(m1, m2) - 1);
        CHECK_EQ(crt(x % m1, m1, x % m2, m2), (pair<ll, ll>{x, lcm(m1, m2)}));
        CHECK_EQ(crt(x % m2, m2, x % m1, m1), (pair<ll, ll>{x, lcm(m1, m2)}));
    }
    // m1 * m2 near 1e18 and coprime
    {
        ll m1 = 999999937, m2 = 1000000007, x = 987654321987654321LL;
        CHECK_EQ(crt(x % m1, m1, x % m2, m2), (pair<ll, ll>{x, m1 * m2}));
    }
    // list version
    CHECK_EQ(crt(vector<ll>{}, vector<ll>{}), (pair<ll, ll>{0, 1}));
    CHECK_EQ(crt(vector<ll>{2, 3, 2}, vector<ll>{3, 5, 7}), (pair<ll, ll>{23, 105}));
    CHECK_EQ(crt(vector<ll>{1, 2, 0}, vector<ll>{2, 4, 3}), (pair<ll, ll>{0, -1}));
    for (int it = 0; it < 1000; it++) {
        int k = (int)test::rnd(1, 5);
        vector<ll> ms(k), rs(k);
        ll x = test::rnd(0, 100000);
        for (int i = 0; i < k; i++) ms[i] = test::rnd(1, 30), rs[i] = x % ms[i];
        ll l = 1;
        for (ll m : ms) l = lcm(l, m);
        CHECK_EQ(crt(rs, ms), (pair<ll, ll>{x % l, l}));
        rs[0] = (rs[0] + 1) % ms[0];
        if (ms[0] > 1 && k > 1) {
            bool ok = false; // still consistent? brute force
            for (ll y = 0; y < l && !ok; y++) {
                ok = true;
                for (int i = 0; i < k; i++) ok &= y % ms[i] == rs[i];
            }
            CHECK_EQ(crt(rs, ms).second == -1, !ok);
        }
    }
}
