#include "test.h"
#include "geometry/point.cpp"
#include "geometry/line-intersection.cpp"

using P = Point<long double>;

static P rp(int c) { return P(test::rnd(-c, c), test::rnd(-c, c)); }

int main() {
    // fixed cases
    auto [k1, p1] = lineInter(P{0, 0}, P{2, 2}, P{0, 2}, P{2, 0});
    CHECK_EQ(k1, 1), CHECK_NEAR(p1.x, 1, 1e-12), CHECK_NEAR(p1.y, 1, 1e-12);
    // segments don't touch, the lines still do
    auto [k2, p2] = lineInter(P{0, 0}, P{1, 0}, P{5, 1}, P{5, 3});
    CHECK_EQ(k2, 1), CHECK_NEAR(p2.x, 5, 1e-12), CHECK_NEAR(p2.y, 0, 1e-12);
    CHECK_EQ(lineInter(P{0, 0}, P{1, 1}, P{0, 1}, P{2, 3}).first, 0);   // parallel
    CHECK_EQ(lineInter(P{0, 0}, P{1, 1}, P{3, 3}, P{-2, -2}).first, -1);  // same line

    for (int it = 0; it < 20000; it++) {
        int c = (int)test::rnd(1, 1000);
        P a = rp(c), b = rp(c), cc = rp(c), d = rp(c);
        if (a == b || cc == d) continue;
        int kind = (int)test::rnd(0, 2);
        if (kind == 1) d = cc + (b - a) * test::rnd(-3, 3);  // parallel or same line
        if (kind == 2) cc = a + (b - a) * test::rnd(-3, 3), d = cc + (b - a) * test::rnd(-3, 3);  // same line
        if (cc == d) continue;
        long long cr = (long long)(b - a).cross(d - cc);
        long long on = (long long)(b - a).cross(cc - a);
        auto [k, p] = lineInter(a, b, cc, d);
        if (cr != 0) {
            CHECK_EQ(k, 1);
            // p is on both lines: the distance to each is ~0
            CHECK_NEAR(a.cross(b, p) / (b - a).dist(), 0, 1e-6);
            CHECK_NEAR(cc.cross(d, p) / (d - cc).dist(), 0, 1e-6);
        } else {
            CHECK_EQ(k, on == 0 ? -1 : 0);
        }
    }
}
