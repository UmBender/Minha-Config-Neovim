#include "test.h"
#include "math/linear-recurrence.cpp"
using ll = long long;

vector<ll> terms(vector<ll> s, const vector<ll> &c, int n, ll p) {
    while ((int)s.size() < n) {
        ll v = 0;
        for (size_t j = 0; j < c.size(); j++) v = (v + c[j] * s[s.size() - 1 - j]) % p;
        s.push_back(v);
    }
    return s;
}

// Fibonacci by fast doubling: (F(k), F(k+1)) mod p
pair<ll, ll> fib(ll k, ll p) {
    if (k == 0) return {0, 1};
    auto [a, b] = fib(k / 2, p);
    ll c = a * ((2 * b - a + p) % p) % p, d = (a * a + b * b) % p;
    return k % 2 ? pair{d, (c + d) % p} : pair{c, d};
}

int main() {
    CHECK_EQ(linRec({0, 1}, {1, 1}, 10), 55LL);
    CHECK_EQ(linRec({0, 1}, {1, 1}, 0), 0LL);
    CHECK_EQ(linRec({5}, {}, 3), 0LL);   // empty recurrence: every term from s.size() on is 0
    CHECK_EQ(linRec({5}, {}, 0), 5LL);
    CHECK_EQ(linRec({3}, {2}, 4, 7), 6LL); // 3 * 2^4 = 48 = 6 (mod 7)
    for (ll p : {2LL, 7LL, 998244353LL, 1000000007LL})
        for (int it = 0; it < 300; it++) {
            int l = (int)test::rnd(1, 12);
            auto c = test::rndVec<ll>(l, 0, p - 1), s = test::rndVec<ll>(l, 0, p - 1);
            auto all = terms(s, c, 300, p);
            for (int k = 0; k < 300; k += (int)test::rnd(1, 20)) CHECK_EQ(linRec(s, c, k, p), all[k]);
        }
    for (int it = 0; it < 100; it++) {
        ll k = test::rnd(0, 1000000000000000000LL);
        CHECK_EQ(linRec({0, 1}, {1, 1}, k, 1000000007), fib(k, 1000000007).first);
    }
    // l = 300, k = 1e18 (O(l^2 log k)): s[i] = 2 s[i-300] has s[k] = 2^(k/300) s[k % 300]
    {
        int l = 300;
        vector<ll> c(l, 0), s(l);
        c[l - 1] = 2;
        for (int i = 0; i < l; i++) s[i] = i + 1;
        ll k = 1000000000000000000LL, want = (k % l) + 1;
        for (ll e = k / l, b = 2; e; e >>= 1, b = b * b % 998244353)
            if (e & 1) want = want * b % 998244353;
        CHECK_EQ(linRec(s, c, k), want);
    }
}
