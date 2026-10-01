#include "test.h"
#include "dsa/fenwick-tree.range.cpp"
using ll = long long;

int main() {
    for (int it = 0; it < 300; it++) {
        int n = (int)test::rnd(1, 40);
        vector<ll> a = test::rndVec<ll>(n, -1e9, 1e9);
        RangeFenwick rf(a);
        RangeFenwick zeros(n);
        static_assert(is_same_v<decltype(zeros), RangeFenwick<ll>>);
        vector<ll> z(n, 0);
        for (int q = 0; q < 200; q++) {
            int l = (int)test::rnd(0, n), r = (int)test::rnd(l, n);
            int type = (int)test::rnd(0, 3);
            if (type == 0) {
                ll x = test::rnd(-1e9, 1e9);
                rf.add(l, r, x), zeros.add(l, r, x);
                for (int i = l; i < r; i++) a[i] += x, z[i] += x;
            } else if (type == 1 && l < n) {
                ll x = test::rnd(-1e9, 1e9);
                rf.set(l, x), a[l] = x;
            } else if (type == 2 && l < n) {
                CHECK_EQ(rf.get(l), a[l]);
                CHECK_EQ(zeros.get(l), z[l]);
            }
            CHECK_EQ(rf.sum(l, r), accumulate(a.begin() + l, a.begin() + r, 0LL));
            CHECK_EQ(rf.sum(r), accumulate(a.begin(), a.begin() + r, 0LL));
            CHECK_EQ(zeros.sum(l, r), accumulate(z.begin() + l, z.begin() + r, 0LL));
        }
    }
    RangeFenwick<int> small(4);
    small.add(1, 3, 5);
    CHECK_EQ(small.sum(4), 10);
    CHECK_EQ(small.get(0), 0);
    RangeFenwick<ll> none(0);
    CHECK_EQ(none.sum(0, 0), 0LL);
}
