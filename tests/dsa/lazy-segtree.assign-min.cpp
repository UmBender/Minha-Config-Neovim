#include "test.h"
#include "dsa/lazy-segtree.assign-min.cpp"
using ll = long long;

int main() {
    for (int it = 0; it < 300; it++) {
        int n = (int)test::rnd(1, 30);
        bool fromVec = test::rnd(0, 1);
        vector<ll> b = fromVec ? test::rndVec<ll>(n, -1e9, 1e9) : vector<ll>(n, 0);
        RangeAssignMin<ll> seg = fromVec ? RangeAssignMin<ll>(b) : RangeAssignMin<ll>(n);
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
            CHECK_EQ(seg.min(l, r), *min_element(b.begin() + l, b.begin() + r));
        }
    }
    RangeAssignMin<int> zeros(5);
    zeros.assign(1, 4, 2);
    CHECK_EQ(zeros.min(0, 5), 0);
    CHECK_EQ(zeros.get(4), 0);
}
