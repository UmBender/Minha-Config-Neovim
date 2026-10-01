#include "test.h"
#include "math/floor-sum.cpp"
using ll = long long;
using i128 = __int128;

ll fdiv(ll a, ll b) { return a / b - ((a % b != 0) && ((a < 0) != (b < 0))); }

array<i128, 3> naive(ll n, ll m, ll a, ll b) {
    array<i128, 3> s{};
    for (ll i = 0; i < n; i++) {
        i128 f = fdiv(a * i + b, m);
        s[0] += f, s[1] += i * f, s[2] += f * f;
    }
    return s;
}

void check(ll n, ll m, ll a, ll b) {
    auto want = naive(n, m, a, b);
    auto [f, g, h] = floorSums(n, m, a, b);
    if (f != want[0] || g != want[1] || h != want[2])
        test::fail(__FILE__, __LINE__, "floorSums(" + to_string(n) + ", " + to_string(m) + ", " + to_string(a) + ", " +
                                           to_string(b) + ")");
    CHECK(floorSum(n, m, a, b) == want[0]);
    CHECK(floorSumI(n, m, a, b) == want[1]);
    CHECK(floorSumSq(n, m, a, b) == want[2]);
}

int main() {
    CHECK(floorSum(0, 5, 3, 2) == 0);
    CHECK(floorSum(4, 10, 6, 3) == 3); // 0 + 0 + 1 + 2
    CHECK(floorSum(6, 5, 4, 3) == 13);
    for (ll n = 0; n <= 12; n++)
        for (ll m = 1; m <= 12; m++)
            for (ll a = -15; a <= 15; a++)
                for (ll b = -15; b <= 15; b++) check(n, m, a, b);
    for (int it = 0; it < 3000; it++) {
        ll n = test::rnd(0, 2000), m = test::rnd(1, 1000000000), a = test::rnd(-1000000000, 1000000000),
           b = test::rnd(-1000000000, 1000000000);
        check(n, m, a, b);
    }
    // Fibonacci-like inputs give the deepest recursion
    {
        ll x = 1, y = 1;
        while (y < 1000000000) tie(x, y) = pair{y, x + y};
        check(1000, y, x, 0), check(2000, x, y - x, 12345);
    }
    // large n: closed form for a = m (sum of i), cross-check with a known identity
    ll n = 1000000000;
    CHECK(floorSum(n, 7, 7, 0) == (i128)n * (n - 1) / 2);
    CHECK(floorSumSq(n, 1, 1, 0) == (i128)n * (n - 1) * (2 * n - 1) / 6);
    CHECK(floorSumI(n, 1, 0, 3) == (i128)3 * n * (n - 1) / 2);
}
