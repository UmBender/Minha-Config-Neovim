#include "test.h"
#include "dsa/sparse-table.cpp"
using ll = long long;

int main() {
    SparseTable one(vector<int>{42}, [](int a, int b) { return min(a, b); });
    CHECK_EQ(one.query(0, 1), 42);

    for (int it = 0; it < 300; it++) {
        int n = (int)test::rnd(1, 70);
        vector<ll> a = test::rndVec<ll>(n, -1e9, 1e9);
        SparseTable mn(a, [](ll x, ll y) { return min(x, y); });
        SparseTable mx(a, [](ll x, ll y) { return max(x, y); });
        SparseTable g(a, [](ll x, ll y) { return gcd(x, y); });
        for (int q = 0; q < 100; q++) {
            int l = (int)test::rnd(0, n - 1), r = (int)test::rnd(l + 1, n);
            CHECK_EQ(mn.query(l, r), *min_element(a.begin() + l, a.begin() + r));
            CHECK_EQ(mx.query(l, r), *max_element(a.begin() + l, a.begin() + r));
            ll want = 0;
            for (int i = l; i < r; i++) want = gcd(want, a[i]);
            CHECK_EQ(g.query(l, r), want);
        }
    }
}
