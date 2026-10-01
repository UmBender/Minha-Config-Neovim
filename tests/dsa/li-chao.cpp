#include "test.h"
#include "dsa/li-chao.cpp"
using ll = long long;

int main() {
    LiChao<ll> empty(-10, 10);
    CHECK_EQ(empty.query(3), LiChao<ll>::NONE);

    for (int it = 0; it < 300; it++) {
        bool big = it % 3 == 0;
        ll lo = big ? -(ll)1e9 : test::rnd(-50, 0), hi = big ? (ll)1e9 : test::rnd(1, 50);
        LiChao<ll> mn(lo, hi);
        LiChao<ll, true> mx(lo, hi);
        struct Seg { ll a, b, l, r; };
        vector<Seg> lines;
        for (int q = 0; q < 80; q++) {
            int type = (int)test::rnd(0, 2);
            ll a = test::rnd(-1e6, 1e6), b = test::rnd(-1e9, 1e9);
            if (type == 0) {
                mn.addLine(a, b), mx.addLine(a, b), lines.push_back({a, b, lo, hi});
            } else if (type == 1) {
                ll l = test::rnd(lo, hi), r = test::rnd(lo, hi);
                if (l > r) swap(l, r);
                mn.addSegment(a, b, l, r), mx.addSegment(a, b, l, r), lines.push_back({a, b, l, r});
            } else {
                ll x = test::rnd(lo, hi - 1);
                ll wantMin = LiChao<ll>::NONE, wantMax = LiChao<ll, true>::NONE;
                for (auto s : lines)
                    if (s.l <= x && x < s.r) wantMin = min(wantMin, s.a * x + s.b), wantMax = max(wantMax, s.a * x + s.b);
                CHECK_EQ(mn.query(x), wantMin);
                CHECK_EQ(mx.query(x), wantMax);
            }
        }
    }
    // doubles
    LiChao<double> d(0, 100);
    d.addLine(0.5, 1), d.addLine(-0.25, 10);
    CHECK_NEAR(d.query(4), 3.0, 1e-12);
    CHECK_NEAR(d.query(40), 0.0, 1e-12);
}
