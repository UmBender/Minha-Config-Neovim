#include "test.h"
#include "dsa/ntt.cpp"
using ll = long long;

vector<ll> naive(const vector<ll> &a, const vector<ll> &b, ll mod) {
    if (a.empty() || b.empty()) return {};
    vector<ll> c(a.size() + b.size() - 1);
    for (size_t i = 0; i < a.size(); i++)
        for (size_t j = 0; j < b.size(); j++) c[i + j] = (ll)((c[i + j] + (__int128)a[i] * b[j]) % mod);
    return c;
}

int main() {
    CHECK(nttConv({}, {1}).empty());
    CHECK_EQ(nttConv({1, 2, 3}, {4, 5}), (vector<ll>{4, 13, 22, 15}));
    for (int it = 0; it < 100; it++) {
        int n = (int)test::rnd(1, 400), m = (int)test::rnd(1, 400);
        auto a = test::rndVec<ll>(n, 0, 998244352), b = test::rndVec<ll>(m, 0, 998244352);
        CHECK_EQ(nttConv(a, b), naive(a, b, 998244353));
    }
    // another NTT prime
    for (int it = 0; it < 30; it++) {
        int n = (int)test::rnd(1, 200), m = (int)test::rnd(1, 200);
        auto a = test::rndVec<ll>(n, 0, 469762048), b = test::rndVec<ll>(m, 0, 469762048);
        CHECK_EQ((nttConv<469762049, 3>(a, b)), naive(a, b, 469762049));
    }
    // arbitrary modulus (3 primes + CRT)
    for (ll mod : {1000000007LL, 1000000009LL, 2LL, 999999937LL, 1LL << 30}) {
        for (int it = 0; it < 20; it++) {
            int n = (int)test::rnd(1, 300), m = (int)test::rnd(1, 300);
            auto a = test::rndVec<ll>(n, 0, mod - 1), b = test::rndVec<ll>(m, 0, mod - 1);
            CHECK_EQ(convMod(a, b, mod), naive(a, b, mod));
        }
    }
    // large: spot-check entries
    int n = 1 << 17;
    auto a = test::rndVec<ll>(n, 0, 1000000006), b = test::rndVec<ll>(n, 0, 1000000006);
    auto c = convMod(a, b, 1000000007);
    CHECK_EQ((int)c.size(), 2 * n - 1);
    for (int k = 0; k < 20; k++) {
        int idx = (int)test::rnd(0, 2 * n - 2);
        __int128 want = 0;
        for (int i = max(0, idx - n + 1); i <= min(idx, n - 1); i++) want = (want + (__int128)a[i] * b[idx - i]) % 1000000007;
        CHECK_EQ(c[idx], (ll)want);
    }
}
