#include "test.h"
#include "dsa/fft.cpp"
using ll = long long;

template <class T> vector<T> naive(const vector<T> &a, const vector<T> &b) {
    if (a.empty() || b.empty()) return {};
    vector<T> c(a.size() + b.size() - 1);
    for (size_t i = 0; i < a.size(); i++)
        for (size_t j = 0; j < b.size(); j++) c[i + j] += a[i] * b[j];
    return c;
}

int main() {
    CHECK(fftConv({}, {1.0}).empty());
    CHECK(fftConvInt({}, {}).empty());
    CHECK_EQ(fftConvInt({1, 2, 3}, {4, 5}), (vector<ll>{4, 13, 22, 15}));

    for (int it = 0; it < 200; it++) {
        int n = (int)test::rnd(1, 300), m = (int)test::rnd(1, 300);
        vector<double> a(n), b(m);
        for (auto &x : a) x = test::rndReal(-10, 10);
        for (auto &x : b) x = test::rndReal(-10, 10);
        auto got = fftConv(a, b), want = naive(a, b);
        CHECK_EQ(got.size(), want.size());
        for (size_t i = 0; i < got.size(); i++) CHECK_NEAR(got[i], want[i], 1e-8);
    }
    // integers: exact while results stay below ~1e15
    for (int it = 0; it < 30; it++) {
        int n = (int)test::rnd(1, 3000), m = (int)test::rnd(1, 3000);
        auto a = test::rndVec<ll>(n, -10000, 10000), b = test::rndVec<ll>(m, -10000, 10000);
        CHECK_EQ(fftConvInt(a, b), naive(a, b));
    }
    auto a = test::rndVec<ll>(1 << 16, 0, 1000), b = test::rndVec<ll>(1 << 16, 0, 1000);
    auto c = fftConvInt(a, b);
    for (int k = 0; k < 50; k++) {
        int idx = (int)test::rnd(0, (ll)c.size() - 1);
        ll want = 0;
        for (int i = max(0, idx - (int)b.size() + 1); i <= min(idx, (int)a.size() - 1); i++) want += a[i] * b[idx - i];
        CHECK_EQ(c[idx], want);
    }
}
