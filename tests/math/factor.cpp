#include "test.h"
#include "math/primality.cpp"
#include "math/factor.cpp"
using ull = unsigned long long;

vector<ull> naive(ull n) {
    vector<ull> fs;
    for (ull d = 2; d * d <= n; d++)
        while (n % d == 0) fs.push_back(d), n /= d;
    if (n > 1) fs.push_back(n);
    return fs;
}

ull randomPrime(ull lo, ull hi) {
    for (;;)
        if (ull p = test::rnd(lo, hi); isPrime(p)) return p;
}

void checkBig(ull n) {
    auto fs = factor(n);
    CHECK(is_sorted(fs.begin(), fs.end()));
    unsigned __int128 prod = 1;
    for (ull p : fs) CHECK(isPrime(p)), prod *= p;
    CHECK(prod == n);
}

int main() {
    CHECK(factor(1).empty());
    CHECK_EQ(factor(2), (vector<ull>{2}));
    CHECK_EQ(factor(12), (vector<ull>{2, 2, 3}));
    CHECK_EQ(factor(1ULL << 63), vector<ull>(63, 2));
    CHECK_EQ(factor(18446744073709551557ULL), (vector<ull>{18446744073709551557ULL}));
    CHECK_EQ(factor(18446744073709551615ULL), (vector<ull>{3, 5, 17, 257, 641, 65537, 6700417}));
    CHECK_EQ(factor(4294967291ULL * 4294967279ULL), (vector<ull>{4294967279ULL, 4294967291ULL}));
    CHECK_EQ(factor(1000000007ULL * 1000000007ULL), (vector<ull>{1000000007ULL, 1000000007ULL}));
    CHECK_EQ(factor(3825123056546413051ULL), (vector<ull>{149491, 747451, 34233211}));
    for (ull n = 1; n < 30000; n++) CHECK_EQ(factor(n), naive(n));
    for (int it = 0; it < 300; it++) {
        ull n = test::rnd(1, 1000000000000LL);
        CHECK_EQ(factor(n), naive(n));
    }
    for (int it = 0; it < 100; it++) checkBig((ull)test::rnd(1, 4000000000000000000LL) * 4 + test::rnd(0, 3));
    // hard cases: products of two (or three) large primes, prime powers
    for (int it = 0; it < 30; it++) {
        ull p = randomPrime(1000000, 4000000000LL), q = randomPrime(1000000, 4000000000LL);
        auto want = vector<ull>{min(p, q), max(p, q)};
        CHECK_EQ(factor(p * q), want);
        ull a = randomPrime(2, 2000000), b = randomPrime(2, 2000000), c = randomPrime(2, 2000000);
        auto fs = vector<ull>{a, b, c};
        sort(fs.begin(), fs.end());
        CHECK_EQ(factor(a * b * c), fs);
        ull r = randomPrime(2, 1000);
        ull pw = 1;
        vector<ull> rs;
        while (pw <= 1000000000000000000ULL / r) pw *= r, rs.push_back(r);
        CHECK_EQ(factor(pw), rs);
    }
}
