#include "test.h"
#include "math/primality.cpp"
using ull = unsigned long long;

bool naive(ull n) {
    if (n < 2) return false;
    for (ull d = 2; d * d <= n; d++)
        if (n % d == 0) return false;
    return true;
}

int main() {
    for (ull n = 0; n < 100000; n++) CHECK_EQ(isPrime(n), naive(n));
    // Carmichael numbers and strong pseudoprimes to small bases
    for (ull n : {561ULL, 1105ULL, 1729ULL, 2047ULL, 3277ULL, 4033ULL, 25326001ULL, 3215031751ULL, 2152302898747ULL,
                  3474749660383ULL, 341550071728321ULL, 3825123056546413051ULL})
        CHECK(!isPrime(n));
    for (ull n : {1000000007ULL, 998244353ULL, 2147483647ULL, 4294967291ULL, 999999999999999989ULL,
                  1000000000000000003ULL, 2305843009213693951ULL, 18446744073709551557ULL})
        CHECK(isPrime(n));
    CHECK(!isPrime(4294967291ULL * 4294967279ULL)); // ~1.8e19, product of two 32-bit primes
    CHECK(!isPrime(18446744073709551615ULL));
    CHECK(!isPrime(1000000007ULL * 1000000007ULL));
    // random numbers up to 1e12 against trial division
    for (int it = 0; it < 300; it++) {
        ull n = test::rnd(1, 1000000000000LL);
        CHECK_EQ(isPrime(n), naive(n));
    }
    // products of two primes are composite
    for (int it = 0; it < 200; it++) {
        auto prime = [&] {
            for (;;)
                if (ull p = test::rnd(2, 4000000000LL); naive(p)) return p;
        };
        ull p = prime(), q = prime();
        CHECK(!isPrime(p * q));
    }
    // helpers
    CHECK_EQ(mulMod64(18446744073709551614ULL, 18446744073709551614ULL, 18446744073709551615ULL), 1ULL);
    CHECK_EQ(powMod64(2, 64, 18446744073709551557ULL), 59ULL); // 2^64 - p = 59
    CHECK_EQ(powMod64(3, 0, 1), 0ULL);
}
