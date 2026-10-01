#include "test.h"
#include "math/powm.cpp"
#include "math/lagrangian-polynomial.cpp"
using ll = long long;

ll eval(const vector<ll> &c, ll x, ll p) {
    ll r = 0;
    x = (x % p + p) % p;
    for (int i = (int)c.size() - 1; i >= 0; i--) r = (r * x + c[i]) % p;
    return r;
}

int main() {
    CHECK(lagrangePoly({}, {}).empty());
    CHECK_EQ(lagrangePoly({3}, {5}), (vector<ll>{5}));
    CHECK_EQ(lagrangePoly({0, 1, 2}, {1, 2, 5}), (vector<ll>{1, 0, 1})); // x^2 + 1
    CHECK_EQ(lagrangePoly({1, 2}, {0, 0}, 7), (vector<ll>{0, 0}));
    CHECK_EQ(lagrangePoly({-1, 1}, {0, 2}, 7), (vector<ll>{1, 1})); // x + 1, negative node
    for (ll p : {2LL, 7LL, 998244353LL, 1000000007LL}) {
        for (int it = 0; it < 300; it++) {
            int n = (int)test::rnd(1, (int)min(p, 25LL));
            auto c = test::rndVec<ll>(n, 0, p - 1);
            set<ll> used;
            vector<ll> xs, ys;
            while ((int)xs.size() < n) {
                ll x = test::rnd(-2 * p, 2 * p);
                if (used.insert((x % p + p) % p).second) xs.push_back(x), ys.push_back(eval(c, x, p));
            }
            CHECK_EQ(lagrangePoly(xs, ys, p), c);
        }
    }
    {
        int n = 2000;
        auto c = test::rndVec<ll>(n, 0, 998244352);
        vector<ll> xs(n), ys(n);
        for (int i = 0; i < n; i++) xs[i] = 3 * i + 1, ys[i] = eval(c, xs[i], 998244353);
        CHECK_EQ(lagrangePoly(xs, ys), c);
    }
}
