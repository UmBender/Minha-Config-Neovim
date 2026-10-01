#include "test.h"
#include "dsa/segtree.cpp"
using ll = long long;

template <class T, class F> void stress(F op, T e, T lo, T hi) {
    for (int it = 0; it < 150; it++) {
        int n = (int)test::rnd(1, 40);
        vector<T> a = test::rndVec<T>(n, lo, hi);
        Segtree seg(a, e, op);
        auto brute = [&](int l, int r) {
            T x = e;
            for (int i = l; i < r; i++) x = op(x, a[i]);
            return x;
        };
        for (int q = 0; q < 150; q++) {
            int type = (int)test::rnd(0, 4);
            if (type == 0) {
                int i = (int)test::rnd(0, n - 1);
                T x = (T)test::rnd((ll)lo, (ll)hi);
                seg.set(i, x), a[i] = x;
            } else if (type == 1) {
                int l = (int)test::rnd(0, n), r = (int)test::rnd(l, n);
                CHECK_EQ(seg.query(l, r), brute(l, r));
                CHECK_EQ(seg.all(), brute(0, n));
            } else if (type == 2) {
                int i = (int)test::rnd(0, n - 1);
                CHECK_EQ(seg.get(i), a[i]);
            } else if (type == 3) {
                // maxRight with a monotone predicate on the running aggregate
                int l = (int)test::rnd(0, n);
                T bound = (T)test::rnd((ll)lo, (ll)hi * 3);
                auto pred = [&](T x) { return x <= bound; };
                int want = l;
                while (want < n && pred(brute(l, want + 1))) want++;
                if (pred(e)) CHECK_EQ(seg.maxRight(l, pred), want);
            } else {
                int r = (int)test::rnd(0, n);
                T bound = (T)test::rnd((ll)lo, (ll)hi * 3);
                auto pred = [&](T x) { return x <= bound; };
                int want = r;
                while (want > 0 && pred(brute(want - 1, r))) want--;
                if (pred(e)) CHECK_EQ(seg.minLeft(r, pred), want);
            }
        }
    }
}

int main() {
    // sum (non-negative so maxRight/minLeft predicates are monotone)
    stress<ll>([](ll x, ll y) { return x + y; }, 0LL, 0LL, 100LL);
    // max
    stress<int>([](int x, int y) { return max(x, y); }, INT_MIN, -50, 50);
    // min with a size-constructed tree
    Segtree mn(5, INT_MAX, [](int x, int y) { return min(x, y); });
    CHECK_EQ(mn.query(0, 5), INT_MAX);
    mn.set(3, 7), mn.set(1, 9);
    CHECK_EQ(mn.query(0, 5), 7);
    CHECK_EQ(mn.query(0, 3), 9);
    CHECK_EQ(mn.query(2, 2), INT_MAX);

    // non-commutative op: string concatenation keeps order
    vector<string> s = {"a", "b", "c", "d"};
    Segtree cat(s, string(), [](const string &x, const string &y) { return x + y; });
    CHECK_EQ(cat.query(1, 4), string("bcd"));
    cat.set(2, "X");
    CHECK_EQ(cat.all(), string("abXd"));

    // struct values: (max, count of max)
    using P = pair<int, int>;
    auto comb = [](P x, P y) {
        if (x.first != y.first) return x.first > y.first ? x : y;
        return P{x.first, x.second + y.second};
    };
    Segtree cnt(vector<P>{{3, 1}, {5, 1}, {5, 1}, {1, 1}}, P{INT_MIN, 0}, comb);
    CHECK_EQ(cnt.all(), P(5, 2));
}
