#include "test.h"
#include "dsa/xor-convolution.cpp"
using ll = long long;

vector<ll> naive(const vector<ll> &a, const vector<ll> &b, ll mod, int kind) {
    int n = 1;
    while (n < (int)max(a.size(), b.size())) n <<= 1;
    vector<ll> c(n);
    for (size_t i = 0; i < a.size(); i++)
        for (size_t j = 0; j < b.size(); j++) {
            int k = kind == 0 ? int(i ^ j) : kind == 1 ? int(i & j) : int(i | j);
            c[k] += mod ? a[i] * b[j] % mod : a[i] * b[j];
            if (mod) c[k] %= mod;
        }
    return c;
}

int main() {
    CHECK_EQ(xorConv({1}, {1}), (vector<ll>{1}));
    for (int it = 0; it < 300; it++) {
        int n = (int)test::rnd(1, 64), m = (int)test::rnd(1, 64);
        auto a = test::rndVec<ll>(n, -1000, 1000), b = test::rndVec<ll>(m, -1000, 1000);
        CHECK_EQ(xorConv(a, b), naive(a, b, 0, 0));
        CHECK_EQ(andConv(a, b), naive(a, b, 0, 1));
        CHECK_EQ(orConv(a, b), naive(a, b, 0, 2));
        const ll mod = 998244353;
        auto am = test::rndVec<ll>(n, 0, mod - 1), bm = test::rndVec<ll>(m, 0, mod - 1);
        CHECK_EQ(xorConv(am, bm, mod), naive(am, bm, mod, 0));
        CHECK_EQ(andConv(am, bm, mod), naive(am, bm, mod, 1));
        CHECK_EQ(orConv(am, bm, mod), naive(am, bm, mod, 2));
    }
}
