#include "test.h"
#include "dsa/max-add-segtree.cpp"
using ll = long long;

int main() {
    MaxAddSegtree<int> z(3);
    CHECK_EQ(z.query(0, 3), 0);
    z.add(1, 2, 5);
    CHECK_EQ(z.query(0, 3), 5);
    CHECK_EQ(z.get(0), 0);

    for (int it = 0; it < 300; it++) {
        int n = (int)test::rnd(1, 40);
        vector<ll> a = test::rndVec<ll>(n, -1e12, 1e12);
        MaxAddSegtree<ll> seg(a);
        for (int q = 0; q < 200; q++) {
            int type = (int)test::rnd(0, 2);
            int l = (int)test::rnd(0, n - 1), r = (int)test::rnd(l + 1, n);
            if (type == 0) {
                ll v = test::rnd(-1e9, 1e9);
                seg.add(l, r, v);
                for (int i = l; i < r; i++) a[i] += v;
            } else if (type == 1) {
                CHECK_EQ(seg.query(l, r), *max_element(a.begin() + l, a.begin() + r));
            } else {
                CHECK_EQ(seg.get(l), a[l]);
            }
        }
    }
}
