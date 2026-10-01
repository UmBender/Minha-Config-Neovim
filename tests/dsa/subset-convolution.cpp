#include "test.h"
#include "dsa/subset-convolution.cpp"
using ll = long long;

vector<ll> naive(const vector<ll> &a, const vector<ll> &b, ll mod) {
    int n = (int)a.size();
    vector<ll> c(n, 0);
    for (int s = 0; s < n; s++)
        for (int t = s;; t = (t - 1) & s) {
            ll x = a[t] * b[s ^ t];
            c[s] = mod ? (c[s] + x % mod) % mod : c[s] + x;
            if (t == 0) break;
        }
    return c;
}

int main() {
    CHECK_EQ(subsetConvolution({3}, {4}), (vector<ll>{12}));
    for (int it = 0; it < 200; it++) {
        int k = (int)test::rnd(0, 9), n = 1 << k;
        auto a = test::rndVec<ll>(n, -1000, 1000), b = test::rndVec<ll>(n, -1000, 1000);
        CHECK_EQ(subsetConvolution(a, b), naive(a, b, 0));
        const ll mod = 998244353;
        auto am = test::rndVec<ll>(n, 0, mod - 1), bm = test::rndVec<ll>(n, 0, mod - 1);
        CHECK_EQ(subsetConvolution(am, bm, mod), naive(am, bm, mod));
    }
}
