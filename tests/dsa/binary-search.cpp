#include "test.h"
#include "dsa/binary-search.cpp"
using ll = long long;

int main() {
    // fixed cases
    CHECK_EQ(firstTrue(0, 10, [](int x) { return x >= 7; }), 7);
    CHECK_EQ(firstTrue(0, 10, [](int) { return false; }), 10);
    CHECK_EQ(firstTrue(0, 10, [](int) { return true; }), 0);
    CHECK_EQ(firstTrue(5, 5, [](int) { return true; }), 5);
    CHECK_EQ(lastTrue(0, 10, [](int x) { return x <= 3; }), 3);
    CHECK_EQ(lastTrue(0, 10, [](int) { return false; }), -1);
    CHECK_EQ(lastTrue(0, 10, [](int) { return true; }), 9);

    // no overflow near the limits (the notebook version computed mid as int)
    const ll big = LLONG_MAX / 2 + 12345;
    CHECK_EQ(firstTrue(LLONG_MIN, LLONG_MAX, [&](ll x) { return x >= big; }), big);
    CHECK_EQ(firstTrue(LLONG_MIN, LLONG_MAX, [&](ll x) { return x >= -big; }), -big);
    CHECK_EQ(firstTrue(0LL, (ll)4e18, [](ll x) { return x >= (ll)3e18 + 7; }), (ll)3e18 + 7);
    CHECK_EQ(firstTrue(0u, UINT_MAX, [](unsigned x) { return x >= 4000000000u; }), 4000000000u);

    // stress: threshold predicates over random ranges, counting evaluations
    for (int it = 0; it < 2000; it++) {
        ll lo = test::rnd(-1000000, 1000000), hi = lo + test::rnd(0, 1000);
        ll thr = test::rnd(lo - 2, hi + 2);
        int calls = 0;
        ll got = firstTrue(lo, hi, [&](ll x) { calls++; return x >= thr; });
        ll want = lo;
        while (want < hi && want < thr) want++;
        CHECK_EQ(got, want);
        CHECK(calls <= 12);
        ll got2 = lastTrue(lo, hi, [&](ll x) { return x < thr; });
        CHECK_EQ(got2, want - 1);
    }

    // reals: sqrt(2) and a boundary close to hi
    CHECK_NEAR(firstTrueReal(0.0, 2.0, [](double x) { return x * x >= 2; }), sqrt(2.0), 1e-9);
    CHECK_NEAR(firstTrueReal(-5.0L, 5.0L, [](long double x) { return x >= 4.75L; }), 4.75L, 1e-12);
}
