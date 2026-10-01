#include "test.h"
#include "math/powm.cpp"
#include "math/berlekamp-massey.cpp"
using ll = long long;

// does s[i] = sum c[j] s[i-1-j] hold for every i >= c.size()?
bool generates(const vector<ll> &c, const vector<ll> &s, ll p) {
    for (size_t i = c.size(); i < s.size(); i++) {
        ll v = 0;
        for (size_t j = 0; j < c.size(); j++) v = (v + c[j] * s[i - 1 - j]) % p;
        if (v != s[i]) return false;
    }
    return true;
}

// shortest recurrence by enumeration (small p and lengths)
int naiveLength(const vector<ll> &s, ll p) {
    for (int l = 0;; l++) {
        vector<ll> c(l, 0);
        for (;;) {
            if (generates(c, s, p)) return l;
            int j = 0;
            while (j < l && c[j] == p - 1) c[j++] = 0;
            if (j == l) break;
            c[j]++;
        }
    }
}

vector<ll> extend(vector<ll> s, const vector<ll> &c, int n, ll p) {
    while ((int)s.size() < n) {
        ll v = 0;
        for (size_t j = 0; j < c.size(); j++) v = (v + c[j] * s[s.size() - 1 - j]) % p;
        s.push_back(v);
    }
    return s;
}

int main() {
    CHECK(berlekampMassey({}).empty());
    CHECK(berlekampMassey({0, 0, 0}).empty());
    CHECK_EQ(berlekampMassey({1, 1, 2, 3, 5, 8, 13}), (vector<ll>{1, 1}));
    CHECK_EQ(berlekampMassey({1, 2, 4, 8, 16}), (vector<ll>{2}));
    CHECK_EQ(berlekampMassey({1, 1000000006, 1, 1000000006}, 1000000007), (vector<ll>{1000000006})); // s[i] = -s[i-1]
    CHECK_EQ(berlekampMassey({0, 0, 1}).size(), 3u);
    // minimal length against enumeration, mod 2 and mod 3
    for (ll p : {2LL, 3LL})
        for (int it = 0; it < 1500; it++) {
            int n = (int)test::rnd(0, 8);
            auto s = test::rndVec<ll>(n, 0, p - 1);
            auto c = berlekampMassey(s, p);
            CHECK(generates(c, s, p));
            for (ll x : c) CHECK(0 <= x && x < p);
            CHECK_EQ((int)c.size(), naiveLength(s, p));
        }
    // large prime: recovers a random recurrence of length l from 2l terms
    for (ll p : {998244353LL, 1000000007LL})
        for (int it = 0; it < 300; it++) {
            int l = (int)test::rnd(1, 40);
            auto c = test::rndVec<ll>(l, 0, p - 1);
            c.back() = test::rnd(1, p - 1);
            auto s = extend(test::rndVec<ll>(l, 0, p - 1), c, 2 * l + (int)test::rnd(0, 10), p);
            auto got = berlekampMassey(s, p);
            CHECK(generates(got, s, p));
            CHECK((int)got.size() <= l);
            CHECK(generates(got, extend(s, c, (int)s.size() + 50, p), p)); // predicts the next terms
        }
    // n = 2000
    {
        int l = 1000;
        auto c = test::rndVec<ll>(l, 0, 998244352);
        auto s = extend(test::rndVec<ll>(l, 0, 998244352), c, 2 * l, 998244353);
        CHECK_EQ(berlekampMassey(s), c); // random: the recurrence is unique w.h.p.
    }
}
