#include "test.h"
#include "dsa/max-add-segtree.min.cpp"
using ll = long long;

int main() {
    MinAddSegtree m(4, 7LL);
    CHECK_EQ(m.query(0, 4), 7LL);
    m.add(1, 3, -2);
    CHECK_EQ(m.query(0, 4), 5LL);
    CHECK_EQ(m.get(3), 7LL);
    for (int it = 0; it < 300; it++) {
        int n = (int)test::rnd(1, 40);
        vector<ll> a = test::rndVec<ll>(n, -1e12, 1e12);
        MinAddSegtree seg(a);
        MinAddSegtree zeros(n);
        static_assert(is_same_v<decltype(zeros), MinAddSegtree<ll>>);
        vector<ll> zv(n, 0);
        for (int q = 0; q < 200; q++) {
            int type = (int)test::rnd(0, 2);
            int l = (int)test::rnd(0, n - 1), r = (int)test::rnd(l + 1, n);
            if (type == 0) {
                ll v = test::rnd(-1e9, 1e9);
                seg.add(l, r, v), zeros.add(l, r, v);
                for (int i = l; i < r; i++) a[i] += v, zv[i] += v;
            } else if (type == 1) {
                CHECK_EQ(seg.query(l, r), *min_element(a.begin() + l, a.begin() + r));
                CHECK_EQ(zeros.query(l, r), *min_element(zv.begin() + l, zv.begin() + r));
            } else {
                CHECK_EQ(seg.get(l), a[l]);
            }
        }
    }
}
