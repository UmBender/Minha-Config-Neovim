#include "test.h"
#include "dsa/treap.cpp"
using ll = long long;

int main() {
    for (int it = 0; it < 150; it++) {
        int n0 = (int)test::rnd(0, 20);
        vector<ll> a = test::rndVec<ll>(n0, -100, 100);
        ImplicitTreap<ll> t(a);
        for (int q = 0; q < 200; q++) {
            int n = (int)a.size();
            CHECK_EQ(t.size(), n);
            int type = (int)test::rnd(0, 7);
            if (type == 0) {
                int p = (int)test::rnd(0, n);
                ll x = test::rnd(-100, 100);
                t.insert(p, x), a.insert(a.begin() + p, x);
            } else if (type == 1 && n > 0) {
                int p = (int)test::rnd(0, n - 1);
                t.erase(p), a.erase(a.begin() + p);
            } else if (type == 2 && n > 0) {
                int p = (int)test::rnd(0, n - 1);
                CHECK_EQ(t.get(p), a[p]);
            } else if (type == 3 && n > 0) {
                int p = (int)test::rnd(0, n - 1);
                ll x = test::rnd(-100, 100);
                t.set(p, x), a[p] = x;
            } else if (type == 4) {
                int l = (int)test::rnd(0, n), r = (int)test::rnd(l, n);
                t.reverse(l, r), reverse(a.begin() + l, a.begin() + r);
            } else if (type == 5) {
                int l = (int)test::rnd(0, n), r = (int)test::rnd(l, n);
                ll x = test::rnd(-50, 50);
                t.add(l, r, x);
                for (int i = l; i < r; i++) a[i] += x;
            } else if (type == 6 && n > 0) {
                int l = (int)test::rnd(0, n - 1), r = (int)test::rnd(l + 1, n);
                CHECK_EQ(t.sum(l, r), accumulate(a.begin() + l, a.begin() + r, 0LL));
                CHECK_EQ(t.min(l, r), *min_element(a.begin() + l, a.begin() + r));
            } else if (type == 7) {
                CHECK_EQ(t.toVector(), a);
                CHECK_EQ(t.sum(0, n), accumulate(a.begin(), a.end(), 0LL));
            }
        }
    }
    // T defaults to long long
    {
        ImplicitTreap d;
        static_assert(is_same_v<decltype(d), ImplicitTreap<ll>>);
        d.insert(0, 3000000000LL), d.insert(0, 1);
        CHECK_EQ(d.sum(0, 2), 3000000001LL);
        CHECK_EQ(d.min(0, 2), 1LL);
    }
    // push_back on a big sequence + split/merge by hand (move a block to the front)
    ImplicitTreap<int> t;
    vector<int> a;
    for (int i = 0; i < 100000; i++) t.insert(t.size(), i), a.push_back(i);
    auto [x, rest] = t.split(t.root, 30000);
    auto [y, z] = t.split(rest, 40000);
    t.root = t.merge(t.merge(y, x), z);
    rotate(a.begin(), a.begin() + 30000, a.begin() + 70000);
    CHECK_EQ(t.toVector(), a);
    CHECK_EQ(t.sum(0, t.size()), 4999950000LL);  // int elements, long long sums
}
