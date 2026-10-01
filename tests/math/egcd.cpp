#include "test.h"
#include "math/egcd.cpp"
using ll = long long;

int main() {
    ll x, y;
    CHECK_EQ(egcd(0, 0, x, y), 0LL);
    CHECK_EQ(egcd(5, 0, x, y), 5LL), CHECK_EQ(5 * x + 0 * y, 5LL);
    CHECK_EQ(egcd(0, 7, x, y), 7LL), CHECK_EQ(7 * y, 7LL);
    CHECK_EQ(egcd(240, 46, x, y), 2LL), CHECK_EQ(240 * x + 46 * y, 2LL);
    for (int it = 0; it < 5000; it++) {
        ll hi = it < 2500 ? 100 : 1000000000000000000LL;
        ll a = test::rnd(0, hi), b = test::rnd(0, hi);
        ll g = egcd(a, b, x, y);
        CHECK_EQ(g, gcd(a, b));
        CHECK_EQ((__int128)a * x + (__int128)b * y, (__int128)g);
        // the coefficients stay small: |x| <= b / g, |y| <= a / g (or 1)
        if (g) CHECK(abs(x) <= max(1LL, b / g) && abs(y) <= max(1LL, a / g));
    }
    // invMod: any modulus
    CHECK_EQ(invMod(3, 7), 5LL);
    CHECK_EQ(invMod(2, 4), -1LL);
    CHECK_EQ(invMod(0, 1), 0LL);
    CHECK_EQ(invMod(-3, 7), 2LL);
    CHECK_EQ(invMod(10, 7), 5LL); // a >= m
    for (int it = 0; it < 5000; it++) {
        ll m = test::rnd(1, it < 2500 ? 60 : 1000000000000000000LL), a = test::rnd(-m, m);
        ll inv = invMod(a, m);
        if (gcd(((a % m) + m) % m, m) != 1) {
            CHECK_EQ(inv, -1LL);
        } else {
            CHECK(0 <= inv && inv < m);
            CHECK_EQ(((__int128)a * inv % m + m) % m, (__int128)(1 % m));
        }
    }
}
