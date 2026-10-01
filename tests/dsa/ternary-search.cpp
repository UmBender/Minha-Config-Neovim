#include "test.h"
#include "dsa/ternary-search.cpp"
using ll = long long;

int main() {
    CHECK_EQ(ternaryMax(-10, 10, [](int x) { return -(x - 3) * (x - 3); }), 3);
    CHECK_EQ(ternaryMin(-10, 10, [](int x) { return (x + 4) * (x + 4); }), -4);
    CHECK_EQ(ternaryMax(5, 5, [](int x) { return x; }), 5);
    // large ll range (the notebook version returned int)
    const ll peak = (ll)1e15 + 3;
    CHECK_EQ(ternaryMax(0LL, (ll)2e15, [&](ll x) { return -llabs(x - peak); }), peak);

    // stress: strictly increasing up to the peak, then non-increasing (plateaus allowed after it)
    for (int it = 0; it < 3000; it++) {
        int n = (int)test::rnd(1, 60), p = (int)test::rnd(0, n - 1);
        vector<ll> f(n);
        f[p] = test::rnd(0, 100);
        for (int i = p - 1; i >= 0; i--) f[i] = f[i + 1] - test::rnd(1, 5);
        for (int i = p + 1; i < n; i++) f[i] = f[i - 1] - test::rnd(0, 5);
        int got = ternaryMax(0, n - 1, [&](int i) { return f[i]; });
        CHECK_EQ(got, p);
        vector<ll> g(n);
        for (int i = 0; i < n; i++) g[i] = -f[i];
        CHECK_EQ(ternaryMin(0, n - 1, [&](int i) { return g[i]; }), p);
    }

    // reals
    CHECK_NEAR(ternaryMaxReal(-5.0, 5.0, [](double x) { return -(x - 1.25) * (x - 1.25); }), 1.25, 1e-6);
    CHECK_NEAR(ternaryMinReal(0.0L, 3.0L, [](long double x) { return fabsl(x - 2.5L); }), 2.5L, 1e-6);
}
