#include "test.h"
#include "geometry/point.cpp"
#include "geometry/segment-intersection.cpp"

using P = Point<long long>;

static bool inBox(P p, P a, P b) {
    return min(a.x, b.x) <= p.x && p.x <= max(a.x, b.x) && min(a.y, b.y) <= p.y && p.y <= max(a.y, b.y);
}
static bool naiveOn(P p, P a, P b) { return (b - a).cross(p - a) == 0 && inBox(p, a, b); }

// parametric brute force: a + t (b - a) = c + u (d - c), t and u in [0, 1] (strictly inside when proper)
static bool naiveInter(P a, P b, P c, P d, bool proper) {
    P r = b - a, s = d - c;
    long long den = r.cross(s);
    if (den == 0) {
        if (proper) return false;
        if (a == b) return naiveOn(a, c, d);
        if (c == d) return naiveOn(c, a, b);
        if (r.cross(c - a) != 0) return false;  // parallel, different lines
        return naiveOn(c, a, b) || naiveOn(d, a, b) || naiveOn(a, c, d) || naiveOn(b, c, d);
    }
    long long tn = (c - a).cross(s), un = (c - a).cross(r);
    if (den < 0) den = -den, tn = -tn, un = -un;
    if (proper) return 0 < tn && tn < den && 0 < un && un < den;
    return 0 <= tn && tn <= den && 0 <= un && un <= den;
}

int main() {
    // fixed cases
    CHECK(segInter(P{0, 0}, P{2, 2}, P{0, 2}, P{2, 0})), CHECK(properInter(P{0, 0}, P{2, 2}, P{0, 2}, P{2, 0}));
    CHECK(segInter(P{0, 0}, P{2, 0}, P{2, 0}, P{3, 5})), CHECK(!properInter(P{0, 0}, P{2, 0}, P{2, 0}, P{3, 5}));
    CHECK(segInter(P{0, 0}, P{4, 0}, P{2, 0}, P{6, 0})), CHECK(!properInter(P{0, 0}, P{4, 0}, P{2, 0}, P{6, 0}));
    CHECK(!segInter(P{0, 0}, P{1, 0}, P{2, 0}, P{3, 0}));  // collinear, disjoint
    CHECK(!segInter(P{0, 0}, P{1, 1}, P{0, 1}, P{1, 2}));  // parallel
    CHECK(segInter(P{1, 1}, P{1, 1}, P{0, 0}, P{2, 2}));   // a point on a segment
    CHECK(!segInter(P{1, 1}, P{1, 1}, P{2, 2}, P{2, 2}));
    CHECK(onSegment(P{1, 1}, P{0, 0}, P{3, 3})), CHECK(onSegment(P{3, 3}, P{0, 0}, P{3, 3}));
    CHECK(!onSegment(P{4, 4}, P{0, 0}, P{3, 3})), CHECK(!onSegment(P{1, 2}, P{0, 0}, P{3, 3}));
    // floating coordinates
    using PD = Point<long double>;
    CHECK(segInter(PD{0, 0}, PD{1, 1}, PD{0, 1}, PD{1, 0})), CHECK(!segInter(PD{0, 0}, PD{1, 1}, PD{2, 0}, PD{3, -1}));
    CHECK(onSegment(PD{0.1L, 0.1L}, PD{0, 0}, PD{0.3L, 0.3L}));  // not exact in binary

    for (int it = 0; it < 200000; it++) {
        int c = (int)test::rnd(1, it % 2 ? 3 : 1000000000);
        auto rp = [&] { return P{test::rnd(-c, c), test::rnd(-c, c)}; };
        P a = rp(), b = rp(), cc = rp(), d = rp();
        CHECK_EQ(segInter(a, b, cc, d), naiveInter(a, b, cc, d, false));
        CHECK_EQ(properInter(a, b, cc, d), naiveInter(a, b, cc, d, true));
        CHECK_EQ(onSegment(cc, a, b), naiveOn(cc, a, b));
    }
}
