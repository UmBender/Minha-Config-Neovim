#include "test.h"
#include "math/powm.cpp"
#include "math/lagrangian-interpolation.cpp"
using ll = long long;

ll eval(const vector<ll> &c, ll x, ll p) {
    ll r = 0;
    x = (x % p + p) % p;
    for (int i = (int)c.size() - 1; i >= 0; i--) r = (r * x + c[i]) % p;
    return r;
}

int main() {
    CHECK_EQ(lagrange(vector<ll>{1, 2, 3}, vector<ll>{1, 4, 9}, 10), 100LL);
    CHECK_EQ(lagrange(vector<ll>{0, 1, 4, 9}, 5), 25LL);
    CHECK_EQ(lagrange(vector<ll>{0, 1, 4, 9}, 2), 4LL); // x is a node
    CHECK_EQ(lagrange(vector<ll>{7}, 123456), 7LL);
    CHECK_EQ(lagrange(vector<ll>{5}, vector<ll>{7}, 3), 7LL);
    for (ll p : {7LL, 13LL, 998244353LL, 1000000007LL}) {
        for (int it = 0; it < 400; it++) {
            int n = (int)test::rnd(1, (int)min(p, 30LL));
            auto c = test::rndVec<ll>(n, 0, p - 1);
            // distinct nodes modulo p (some given negative or >= p)
            set<ll> used;
            vector<ll> xs, ys;
            while ((int)xs.size() < n) {
                ll x = test::rnd(-3 * p, 3 * p);
                if (used.insert((x % p + p) % p).second) xs.push_back(x), ys.push_back(eval(c, x, p));
            }
            ll x = it % 5 ? test::rnd(-1000000000000000000LL, 1000000000000000000LL) : xs[test::rnd(0, n - 1)];
            CHECK_EQ(lagrange(xs, ys, x, p), eval(c, x, p));
            // consecutive nodes 0..n-1
            vector<ll> zs(n);
            for (int i = 0; i < n; i++) zs[i] = eval(c, i, p);
            CHECK_EQ(lagrange(zs, x, p), eval(c, x, p));
        }
    }
    // O(n) version on a large input: sum of i^3 for i < x is a degree-4 polynomial
    {
        int n = 200000;
        vector<ll> ys(n);
        auto s3 = [](ll x) { // (x(x-1)/2)^2 mod p, x >= 0
            const ll p = 998244353;
            ll t = (__int128)x * (x - 1) / 2 % p;
            return t * t % p;
        };
        for (int i = 0; i < n; i++) ys[i] = s3(i);
        CHECK_EQ(lagrange(ys, 1000000000000LL), s3(1000000000000LL));
    }
}
