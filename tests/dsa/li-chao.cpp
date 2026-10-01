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
    // presets: T = long long by default, default range x in [-1e9, 1e9]
    {
        LiChao def;
        static_assert(is_same_v<decltype(def), LiChao<ll, false>>);
        MinLiChao mn;
        MaxLiChao mx;
        static_assert(is_same_v<decltype(mn), LiChao<ll, false>>);
        static_assert(is_same_v<decltype(mx), LiChao<ll, true>>);
        MaxLiChao<int> small(0, 10);
        static_assert(is_same_v<decltype(small), LiChao<int, true>>);
        small.addLine(2, 1);
        CHECK_EQ(small.query(9), 19);
        const ll C = (ll)1e9;
        {  // the default range is closed: segments reach x = 1e9 (and stop before it when asked)
            MinLiChao seg;
            seg.addSegment(1, 0, C - 5, C + 1);
            seg.addSegment(-1, 0, -C, -C + 1);
            CHECK_EQ(seg.query(C), C);
            CHECK_EQ(seg.query(-C), C);
            CHECK_EQ(seg.query(0), seg.NONE);
            MaxLiChao part;
            part.addSegment(1, 0, C - 5, C);
            CHECK_EQ(part.query(C - 1), C - 1);
            CHECK_EQ(part.query(C), part.NONE);
        }
        vector<pair<ll, ll>> lines;
        for (int q = 0; q < 3000; q++) {
            if (q % 2 == 0) {
                ll a = test::rnd(-1e6, 1e6), b = test::rnd(-1e9, 1e9);
                def.addLine(a, b), mn.addLine(a, b), mx.addLine(a, b), lines.push_back({a, b});
            } else {
                ll x = q % 10 == 1 ? -C : q % 10 == 3 ? C : test::rnd(-C, C);
                ll wantMin = LLONG_MAX, wantMax = LLONG_MIN;
                for (auto [a, b] : lines) wantMin = min(wantMin, a * x + b), wantMax = max(wantMax, a * x + b);
                CHECK_EQ(def.query(x), wantMin);
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
