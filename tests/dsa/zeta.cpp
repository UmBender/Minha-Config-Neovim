#include "test.h"
#include "dsa/zeta.cpp"
using ll = long long;

int main() {
    for (int it = 0; it < 200; it++) {
        int k = (int)test::rnd(0, 7), n = 1 << k;
        auto a = test::rndVec<ll>(n, -100, 100);
        vector<ll> sub(n, 0), sup(n, 0), subMax(n, LLONG_MIN);
        for (int s = 0; s < n; s++)
            for (int t = 0; t < n; t++) {
                if ((t & s) == t) sub[s] += a[t], subMax[s] = max(subMax[s], a[t]);
                if ((t & s) == s) sup[s] += a[t];
            }
        CHECK_EQ(subsetZeta(a), sub);
        CHECK_EQ(supersetZeta(a), sup);
        CHECK_EQ(subsetMobius(sub), a);
        CHECK_EQ(supersetMobius(sup), a);
        CHECK_EQ(subsetZeta(a, [](ll x, ll y) { return max(x, y); }), subMax);
    }
    // divisor / multiple transforms on indices 1..n (index 0 untouched)
    for (int it = 0; it < 200; it++) {
        int n = (int)test::rnd(0, 120);
        auto a = test::rndVec<ll>(n + 1, -100, 100);
        vector<ll> div(n + 1, 0), mul(n + 1, 0);
        div[0] = mul[0] = a[0];
        for (int m = 1; m <= n; m++)
            for (int d = 1; d <= n; d++)
                if (m % d == 0) div[m] += a[d], mul[d] += a[m];
        CHECK_EQ(divisorZeta(a), div);
        CHECK_EQ(multipleZeta(a), mul);
        CHECK_EQ(divisorMobius(div), a);
        CHECK_EQ(multipleMobius(mul), a);
    }
    // doubles work as T
    vector<double> d = {1.5, 2.5};
    CHECK_EQ(subsetZeta(d), (vector<double>{1.5, 4.0}));
}
