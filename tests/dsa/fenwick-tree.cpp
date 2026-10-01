#include "test.h"
#include "dsa/fenwick-tree.cpp"
using ll = long long;

int main() {
    // empty and single element
    Fenwick<ll> e(0);
    CHECK_EQ(e.sum(0), 0LL);
    Fenwick<int> one(1);
    one.add(0, 5);
    CHECK_EQ(one.sum(0, 1), 5);
    CHECK_EQ(one.get(0), 5);

    for (int it = 0; it < 300; it++) {
        int n = (int)test::rnd(1, 40);
        vector<ll> a = test::rndVec<ll>(n, -1000000000000LL, 1000000000000LL);
        Fenwick<ll> fw(a);
        for (int q = 0; q < 200; q++) {
            int type = (int)test::rnd(0, 3);
            if (type == 0) {
                int i = (int)test::rnd(0, n - 1);
                ll x = test::rnd(-1e12, 1e12);
                fw.add(i, x), a[i] += x;
            } else if (type == 1) {
                int i = (int)test::rnd(0, n - 1);
                ll x = test::rnd(-1e12, 1e12);
                fw.set(i, x), a[i] = x;
            } else if (type == 2) {
                int i = (int)test::rnd(0, n - 1);
                CHECK_EQ(fw.get(i), a[i]);
            } else {
                int l = (int)test::rnd(0, n), r = (int)test::rnd(l, n);
                CHECK_EQ(fw.sum(l, r), accumulate(a.begin() + l, a.begin() + r, 0LL));
                CHECK_EQ(fw.sum(r), accumulate(a.begin(), a.begin() + r, 0LL));
            }
        }
    }

    // lowerBound: smallest r with sum(r) >= s (non-negative values), n + 1 if none
    for (int it = 0; it < 300; it++) {
        int n = (int)test::rnd(1, 40);
        vector<int> a = test::rndVec<int>(n, 0, 5);
        Fenwick<int> fw(n);
        for (int i = 0; i < n; i++) fw.add(i, a[i]);
        int total = accumulate(a.begin(), a.end(), 0);
        for (int s = -1; s <= total + 2; s++) {
            int want = 0, pre = 0;
            while (want <= n && pre < s) {
                if (want == n) { want++; break; }
                pre += a[want++];
            }
            CHECK_EQ(fw.lowerBound(s), want);
        }
    }

    // works with doubles too
    Fenwick<double> fd(3);
    fd.add(1, 0.5), fd.add(2, 0.25);
    CHECK_NEAR(fd.sum(0, 3), 0.75, 1e-12);

    // T defaults to long long
    Fenwick big(3);
    static_assert(is_same_v<decltype(big), Fenwick<ll>>);
    big.add(0, 4e12), big.add(2, 4e12);
    CHECK_EQ(big.sum(0, 3), 8000000000000LL);
}
