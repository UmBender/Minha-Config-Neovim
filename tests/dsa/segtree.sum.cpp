#include "test.h"
#include "dsa/segtree.sum.cpp"
using ll = long long;

ll brute(const vector<ll> &a, int l, int r) {
    ll x = 0;
    for (int i = l; i < r; i++) {
        ll y = a[i];
        x = x + y;
    }
    return x;
}

int main() {
    for (int it = 0; it < 300; it++) {
        int n = (int)test::rnd(1, 40);
        bool fromVec = test::rnd(0, 1);
        vector<ll> a = fromVec ? test::rndVec<ll>(n, -1e12, 1e12) : vector<ll>(n, 0);
        SumSegtree<> seg = fromVec ? SumSegtree<>(a) : SumSegtree<>(n);
        for (int q = 0; q < 150; q++) {
            int type = (int)test::rnd(0, 4);
            int i = (int)test::rnd(0, n - 1);
            ll x = test::rnd(-1e12, 1e12);
            if (type == 0) seg.set(i, x), a[i] = x;
            else if (type == 1) CHECK_EQ(seg.get(i), a[i]);
            else if (type == 4) seg.add(i, x), a[i] += x;
            else {
                int l = (int)test::rnd(0, n), r = (int)test::rnd(l, n);
                CHECK_EQ(seg.query(l, r), brute(a, l, r));
            }
        }
    }
    SumSegtree<int> small(vector<int>{4, 2, 7});
    CHECK_EQ(small.query(0, 3), 13);
    CHECK_EQ(small.query(1, 1), 0);
}
