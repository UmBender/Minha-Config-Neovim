#include "test.h"
#include "math/powm.cpp"
#include "math/mod-sqrt.cpp"
using ll = long long;

bool prime(ll n) {
    if (n < 2) return false;
    for (ll d = 2; d * d <= n; d++)
        if (n % d == 0) return false;
    return true;
}

int main() {
    CHECK_EQ(sqrtMod(0, 7), 0LL);
    CHECK_EQ(sqrtMod(1, 2), 1LL);
    CHECK_EQ(sqrtMod(0, 2), 0LL);
    CHECK_EQ(sqrtMod(2, 7), 3LL);
    CHECK_EQ(sqrtMod(3, 7), -1LL);
    CHECK_EQ(sqrtMod(9, 7), 3LL); // a >= p is reduced
    CHECK_EQ(sqrtMod(-1, 5), 2LL);
    // every residue of small primes against brute force
    for (ll p = 2; p < 400; p++) {
        if (!prime(p)) continue;
        for (ll a = 0; a < p; a++) {
            ll want = -1;
            for (ll x = 0; x < p && want < 0; x++)
                if (x * x % p == a) want = x;
            CHECK_EQ(sqrtMod(a, p), want);
        }
    }
    // large primes (incl. p - 1 divisible by a large power of two): squares have the smallest root
    for (ll p : {998244353LL, 1000000007LL, 469762049LL, 167772161LL, 2013265921LL, 2147483647LL, 754974721LL}) {
        for (int it = 0; it < 300; it++) {
            ll x = test::rnd(0, p - 1);
            ll got = sqrtMod(x * x % p, p);
            CHECK_EQ(got, min(x, p - x));
            ll a = test::rnd(0, p - 1), r = sqrtMod(a, p);
            if (r >= 0) CHECK(r * r % p == a && r <= p - r);
            else CHECK_EQ(powMod(a, (p - 1) / 2, p), p - 1); // non-residue by Euler's criterion
        }
    }
}
