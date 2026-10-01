#include "test.h"
#include "dsa/lazy-segtree.assign-sum.cpp"
using ll = long long;

int main() {
    for (int it = 0; it < 300; it++) {
        int n = (int)test::rnd(1, 30);
        bool fromVec = test::rnd(0, 1);
        vector<ll> b = fromVec ? test::rndVec<ll>(n, -1e9, 1e9) : vector<ll>(n, 0);
        RangeAssignSum<ll> seg = fromVec ? RangeAssignSum<ll>(b) : RangeAssignSum<ll>(n);
        for (int q = 0; q < 150; q++) {
            int l = (int)test::rnd(0, n - 1), r = (int)test::rnd(l + 1, n);
            ll x = test::rnd(-1e9, 1e9);
            int type = (int)test::rnd(0, 3);
            if (type == 0) {
                seg.assign(l, r, x);
                for (int i = l; i < r; i++) b[i] = x;
            } else if (type == 1) {
                seg.set(l, x), b[l] = x;
            } else if (type == 2) {
                CHECK_EQ(seg.get(l), b[l]);
            }
            CHECK_EQ(seg.sum(l, r), accumulate(b.begin() + l, b.begin() + r, 0LL));
        }
    }
    RangeAssignSum<int> zeros(5);
    zeros.assign(1, 4, 2);
    CHECK_EQ(zeros.sum(0, 5), 6);
    CHECK_EQ(zeros.get(4), 0);
}
