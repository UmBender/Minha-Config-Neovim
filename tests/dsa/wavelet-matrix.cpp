#include "test.h"
#include "dsa/wavelet-matrix.cpp"
using ll = long long;

int main() {
    for (int it = 0; it < 200; it++) {
        int n = (int)test::rnd(1, 50);
        ll range = it % 2 ? 5 : (ll)1e18;
        vector<ll> a = test::rndVec<ll>(n, -range, range);
        WaveletMatrix<ll> wm(a);
        for (int q = 0; q < 60; q++) {
            int l = (int)test::rnd(0, n), r = (int)test::rnd(l, n);
            vector<ll> s(a.begin() + l, a.begin() + r);
            sort(s.begin(), s.end());
            if (l < r) {
                int k = (int)test::rnd(0, r - l - 1);
                CHECK_EQ(wm.kth(l, r, k), s[k]);
            }
            ll x = test::rnd(-range - 1, range + 1), y = test::rnd(-range - 1, range + 1);
            if (q % 3 == 0 && l < r) x = s[test::rnd(0, r - l - 1)];  // hit existing values
            if (x > y) swap(x, y);
            int less = (int)(lower_bound(s.begin(), s.end(), x) - s.begin());
            int inRange = (int)(lower_bound(s.begin(), s.end(), y) - s.begin()) - less;
            CHECK_EQ(wm.countLess(l, r, x), less);
            CHECK_EQ(wm.count(l, r, x, y), inRange);
            auto pv = wm.prev(l, r, x);
            CHECK_EQ(pv.has_value(), less > 0);
            if (pv) CHECK_EQ(*pv, s[less - 1]);
            auto nx = wm.next(l, r, x);
            CHECK_EQ(nx.has_value(), less < (int)s.size());
            if (nx) CHECK_EQ(*nx, s[less]);
            // sums wrap internally: exact whenever the answer fits in long long
            __int128 wantSum = 0;
            for (ll v : s) if (x <= v && v < y) wantSum += v;
            if (wantSum == (ll)wantSum) CHECK_EQ(wm.sum(l, r, x, y), (ll)wantSum);
            // visit: disjoint level ranges covering exactly the elements of [l, r) with value in [x, y)
            vector<int> inv_seen;
            wm.visit(l, r, x, y, [&](int h, int lo, int hi) {
                for (int p = lo; p < hi; p++) {
                    int i = -1;
                    for (int j = 0; j < n; j++)
                        if (wm.pos[h][j] == p) i = j;
                    CHECK(i != -1);
                    inv_seen.push_back(i);
                }
            });
            sort(inv_seen.begin(), inv_seen.end());
            vector<int> want;
            for (int i = l; i < r; i++)
                if (x <= a[i] && a[i] < y) want.push_back(i);
            CHECK_EQ(inv_seen, want);
        }
    }
    // CTAD: the value type comes from the vector
    vector<int> small = {5, 1, 4};
    WaveletMatrix ctad(small);
    static_assert(is_same_v<decltype(ctad), WaveletMatrix<int>>);
    CHECK_EQ(ctad.kth(0, 3, 1), 4);

    // single distinct value
    WaveletMatrix<int> same(vector<int>(5, 7));
    CHECK_EQ(same.kth(1, 4, 2), 7);
    CHECK_EQ(same.count(0, 5, 7, 8), 5);
}
