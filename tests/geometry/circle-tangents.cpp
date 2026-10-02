#include "test.h"
#include "geometry/point.cpp"
#include "geometry/circle-tangents.cpp"

using P = Point<long double>;

// expected number of common tangent lines
static size_t want(long double d, long double r1, long double r2) {
    const long double e = 1e-9;
    if (d < e) return 0;  // concentric (or the same circle)
    if (r2 < e) return d > r1 + e ? 2 : d > r1 - e ? 1 : 0;
    if (d > r1 + r2 + e) return 4;
    if (d > r1 + r2 - e) return 3;
    if (d > fabsl(r1 - r2) + e) return 2;
    if (d > fabsl(r1 - r2) - e) return 1;
    return 0;
}

static void check(P c1, long double r1, P c2, long double r2) {
    auto res = circleTangents(c1, r1, c2, r2);
    CHECK_EQ(res.size(), want((c2 - c1).dist(), r1, r2));
    for (auto [p1, p2] : res) {
        CHECK_NEAR((p1 - c1).dist(), r1, 1e-9), CHECK_NEAR((p2 - c2).dist(), r2, 1e-9);
        // the radius to p1 is normal to the line; p2 is on the line at p1, with a parallel radius
        P n = (p1 - c1) / r1;
        CHECK_NEAR((p2 - p1).dot(n), 0, 1e-9), CHECK_NEAR((p2 - c2).cross(n), 0, 1e-9);
    }
    for (int i = 0; i < (int)res.size(); i++)  // distinct lines
        for (int j = 0; j < i; j++)
            CHECK((res[i].first - res[j].first).dist() > 1e-6 || (res[i].second - res[j].second).dist() > 1e-6);
}

int main() {
    // fixed cases
    check(P{0, 0}, 1, P{5, 0}, 2);  // 4
    check(P{0, 0}, 2, P{5, 0}, 3);  // 3: touching outside
    check(P{0, 0}, 2, P{3, 0}, 2);  // 2: crossing
    check(P{0, 0}, 5, P{2, 0}, 3);  // 1: touching inside
    check(P{0, 0}, 5, P{1, 0}, 1);  // 0: nested
    check(P{0, 0}, 5, P{0, 0}, 5);  // 0: the same circle
    check(P{0, 0}, 5, P{0, 0}, 3);  // 0: concentric
    check(P{0, 0}, 3, P{5, 0}, 0);  // tangents from a point: 2
    check(P{0, 0}, 3, P{3, 0}, 0);  // a point on the circle: 1
    check(P{0, 0}, 3, P{1, 0}, 0);  // a point inside: 0
    auto ext = circleTangents(P{0, 0}, 1.0L, P{4, 0}, 1.0L);  // equal radii: y = +-1 are tangents
    CHECK_EQ(ext.size(), 4u);
    int horizontal = 0;
    for (auto [p1, p2] : ext) horizontal += fabsl(p1.y - p2.y) < 1e-12 && fabsl(fabsl(p1.y) - 1) < 1e-12;
    CHECK_EQ(horizontal, 2);
    CHECK_EQ(circleTangents(Point<double>{0, 0}, 1.0, Point<double>{3, 0}, 1.0).size(), 4u);

    for (int it = 0; it < 20000; it++) {
        auto rp = [] { return P(test::rnd(-50, 50), test::rnd(-50, 50)); };
        P c1 = rp(), c2 = rp();
        long double r1 = test::rnd(1, 40), r2 = test::rnd(0, 40), d = (c2 - c1).dist();
        int kind = (int)test::rnd(0, 4);
        if (kind == 1 && d > r1) r2 = d - r1;  // touching outside
        if (kind == 2 && d > 0) r2 = d + r1;   // touching inside
        if (kind == 3) r2 = 0;                 // a point
        if (kind == 4) r2 = r1;                // equal radii
        // integer centers and radii: d is either an integer or far (> 1e-3) from one, so the only
        // near-tangent cases are the exact ones built above
        check(c1, r1, c2, r2);
    }
}
