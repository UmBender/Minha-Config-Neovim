#include "test.h"
#include "dsa/lazy-segtree.cpp"
using ll = long long;
const ll MOD = 998244353;

int main() {
    // range affine (x -> b x + c) + range sum mod p, node = (sum, length)
    struct Node { ll sum, len; };
    struct Aff { ll b, c; };
    for (int it = 0; it < 150; it++) {
        int n = (int)test::rnd(1, 30);
        vector<ll> a = test::rndVec<ll>(n, 0, MOD - 1);
        vector<Node> init(n);
        for (int i = 0; i < n; i++) init[i] = {a[i], 1};
        LazySegtree seg(
            init, Node{0, 0}, Aff{1, 0},
            [](Node x, Node y) { return Node{(x.sum + y.sum) % MOD, x.len + y.len}; },
            [](Aff f, Node x) { return Node{(f.b * x.sum + f.c * x.len) % MOD, x.len}; },
            [](Aff f, Aff g) { return Aff{f.b * g.b % MOD, (f.b * g.c + f.c) % MOD}; });
        for (int q = 0; q < 150; q++) {
            int type = (int)test::rnd(0, 4);
            if (type == 0) {
                int l = (int)test::rnd(0, n), r = (int)test::rnd(l, n);
                Aff f{test::rnd(0, MOD - 1), test::rnd(0, MOD - 1)};
                seg.apply(l, r, f);
                for (int i = l; i < r; i++) a[i] = (f.b * a[i] + f.c) % MOD;
            } else if (type == 1) {
                int i = (int)test::rnd(0, n - 1);
                Aff f{test::rnd(0, MOD - 1), test::rnd(0, MOD - 1)};
                seg.apply(i, f);
                a[i] = (f.b * a[i] + f.c) % MOD;
            } else if (type == 2) {
                int i = (int)test::rnd(0, n - 1);
                ll x = test::rnd(0, MOD - 1);
                seg.set(i, Node{x, 1}), a[i] = x;
            } else if (type == 3) {
                int i = (int)test::rnd(0, n - 1);
                CHECK_EQ(seg.get(i).sum, a[i]);
            } else {
                int l = (int)test::rnd(0, n), r = (int)test::rnd(l, n);
                ll want = 0;
                for (int i = l; i < r; i++) want = (want + a[i]) % MOD;
                CHECK_EQ(seg.query(l, r).sum, want);
                CHECK_EQ(seg.query(l, r).len, (ll)(r - l));
            }
        }
    }

    // range add + range max, with maxRight / minLeft
    for (int it = 0; it < 150; it++) {
        int n = (int)test::rnd(1, 30);
        vector<ll> a = test::rndVec<ll>(n, -20, 20);
        LazySegtree seg(
            a, LLONG_MIN, 0LL, [](ll x, ll y) { return max(x, y); },
            [](ll f, ll x) { return x == LLONG_MIN ? x : x + f; }, [](ll f, ll g) { return f + g; });
        LazySegtree empty(n, LLONG_MIN, 0LL, [](ll x, ll y) { return max(x, y); },
                          [](ll f, ll x) { return x == LLONG_MIN ? x : x + f; }, [](ll f, ll g) { return f + g; });
        CHECK_EQ(empty.all(), LLONG_MIN);
        for (int q = 0; q < 150; q++) {
            int type = (int)test::rnd(0, 3);
            if (type == 0) {
                int l = (int)test::rnd(0, n), r = (int)test::rnd(l, n);
                ll v = test::rnd(-10, 10);
                seg.apply(l, r, v);
                for (int i = l; i < r; i++) a[i] += v;
            } else if (type == 1) {
                int l = (int)test::rnd(0, n), r = (int)test::rnd(l, n);
                ll want = LLONG_MIN;
                for (int i = l; i < r; i++) want = max(want, a[i]);
                CHECK_EQ(seg.query(l, r), want);
                CHECK_EQ(seg.all(), *max_element(a.begin(), a.end()));
            } else if (type == 2) {
                int l = (int)test::rnd(0, n);
                ll bound = test::rnd(-30, 30);
                auto pred = [&](ll x) { return x <= bound; };
                int want = l;
                while (want < n && a[want] <= bound) want++;
                CHECK_EQ(seg.maxRight(l, pred), want);
            } else {
                int r = (int)test::rnd(0, n);
                ll bound = test::rnd(-30, 30);
                auto pred = [&](ll x) { return x <= bound; };
                int want = r;
                while (want > 0 && a[want - 1] <= bound) want--;
                CHECK_EQ(seg.minLeft(r, pred), want);
            }
        }
    }

    // presets: compare all of them against a plain array
    for (int it = 0; it < 200; it++) {
        int n = (int)test::rnd(1, 30);
        auto a = test::rndVec<ll>(n, -1e9, 1e9);
        RangeAddSum<ll> addSum(a);
        RangeAddMin<ll> addMin(a);
        RangeAddMax<ll> addMax(a);
        RangeAssignSum<ll> asgSum(a);
        RangeAssignMin<ll> asgMin(a);
        RangeAssignMax<ll> asgMax(a);
        vector<ll> b = a;  // array for the add presets
        vector<ll> c = a;  // array for the assign presets
        for (int q = 0; q < 150; q++) {
            int l = (int)test::rnd(0, n - 1), r = (int)test::rnd(l + 1, n);
            ll x = test::rnd(-1e9, 1e9);
            int type = (int)test::rnd(0, 3);
            if (type == 0) {
                addSum.add(l, r, x), addMin.add(l, r, x), addMax.add(l, r, x);
                for (int i = l; i < r; i++) b[i] += x;
            } else if (type == 1) {
                asgSum.assign(l, r, x), asgMin.assign(l, r, x), asgMax.assign(l, r, x);
                for (int i = l; i < r; i++) c[i] = x;
            } else if (type == 2) {
                addSum.set(l, x), addMin.set(l, x), addMax.set(l, x), b[l] = x;
                asgSum.set(l, x), asgMin.set(l, x), asgMax.set(l, x), c[l] = x;
            } else {
                CHECK_EQ(addSum.get(l), b[l]);
                CHECK_EQ(addMin.get(l), b[l]);
                CHECK_EQ(asgMax.get(l), c[l]);
            }
            CHECK_EQ(addSum.sum(l, r), accumulate(b.begin() + l, b.begin() + r, 0LL));
            CHECK_EQ(addMin.min(l, r), *min_element(b.begin() + l, b.begin() + r));
            CHECK_EQ(addMax.max(l, r), *max_element(b.begin() + l, b.begin() + r));
            CHECK_EQ(asgSum.sum(l, r), accumulate(c.begin() + l, c.begin() + r, 0LL));
            CHECK_EQ(asgMin.min(l, r), *min_element(c.begin() + l, c.begin() + r));
            CHECK_EQ(asgMax.max(l, r), *max_element(c.begin() + l, c.begin() + r));
        }
    }
    RangeAddSum<int> zeros(5);
    zeros.add(1, 4, 2);
    CHECK_EQ(zeros.sum(0, 5), 6);
}
