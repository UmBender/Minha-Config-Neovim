#include "test.h"
#include "math/powm.cpp"
using ll = long long;

ll naive(ll b, ll e, ll mod) {
    ll r = 1 % mod;
    b = (b % mod + mod) % mod;
    while (e--) r = (__int128)r * b % mod;
    return r;
}

int main() {
    CHECK_EQ(powMod(2, 10), 1024LL);
    CHECK_EQ(powMod(5, 0), 1LL);
    CHECK_EQ(powMod(0, 0), 1LL);
    CHECK_EQ(powMod(7, 5, 1), 0LL);                     // everything is 0 mod 1
    CHECK_EQ(powMod(7, 0, 1), 0LL);
    CHECK_EQ(powMod(-1, 3, 1000000007), 1000000006LL); // negative base
    CHECK_EQ(powMod(3, 998244352), 1LL);               // Fermat, default modulus 998244353
    CHECK_EQ(powMod(1000000006, 2, 1000000007), 1LL);
    CHECK_EQ(powMod(2, 1000000000000000000LL, 1000000007), naive(2, 1000000000000000000LL % 500000003, 1000000007));
    for (int it = 0; it < 3000; it++) {
        ll mod = test::rnd(1, it < 1500 ? 50 : 2147483647LL), b = test::rnd(-1000000000000LL, 1000000000000LL);
        ll e = test::rnd(0, 200);
        CHECK_EQ(powMod(b, e, mod), naive(b, e, mod));
    }
    // inverse modulo a prime
    for (ll p : {2LL, 3LL, 1000000007LL, 998244353LL, 2147483647LL})
        for (int it = 0; it < 100; it++) {
            ll a = test::rnd(1, p - 1);
            CHECK_EQ((__int128)a * powMod(a, p - 2, p) % p, (__int128)1);
        }
}
