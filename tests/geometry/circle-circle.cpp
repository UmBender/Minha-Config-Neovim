#include "test.h"
#include "geometry/point.cpp"
#include "geometry/circle-circle.cpp"

using P = Point<long double>;

int main() {
    // fixed cases
    auto two = circleCircle(P{0, 0}, 5.0L, P{8, 0}, 5.0L);
    CHECK_EQ(two.size(), 2u);
    CHECK_NEAR(two[0].x, 4, 1e-12), CHECK_NEAR(two[1].x, 4, 1e-12);
    CHECK_NEAR(fabsl(two[0].y), 3, 1e-12), CHECK_NEAR(two[0].y + two[1].y, 0, 1e-12);
    auto out = circleCircle(P{0, 0}, 2.0L, P{5, 0}, 3.0L);  // touching outside
    CHECK_EQ(out.size(), 1u), CHECK_NEAR(out[0].x, 2, 1e-12), CHECK_NEAR(out[0].y, 0, 1e-12);
    auto in = circleCircle(P{0, 0}, 5.0L, P{2, 0}, 3.0L);  // touching inside
    CHECK_EQ(in.size(), 1u), CHECK_NEAR(in[0].x, 5, 1e-12), CHECK_NEAR(in[0].y, 0, 1e-12);
    CHECK_EQ(circleCircle(P{0, 0}, 1.0L, P{5, 0}, 1.0L).size(), 0u);  // apart
    CHECK_EQ(circleCircle(P{0, 0}, 5.0L, P{1, 0}, 1.0L).size(), 0u);  // nested
    CHECK_EQ(circleCircle(P{0, 0}, 5.0L, P{0, 0}, 3.0L).size(), 0u);  // concentric
    CHECK_EQ(circleCircle(P{1, 1}, 5.0L, P{1, 1}, 5.0L).size(), 0u);  // the same circle
    CHECK_EQ(circleCircle(Point<double>{0, 0}, 1.0, Point<double>{1, 0}, 1.0).size(), 2u);

    for (int it = 0; it < 20000; it++) {
        auto rp = [] { return P(test::rnd(-50, 50), test::rnd(-50, 50)); };
        P c1 = rp(), c2 = rp();
        long double r1 = test::rnd(1, 60), r2 = test::rnd(1, 60), d = (c2 - c1).dist();
        int kind = (int)test::rnd(0, 3);
        if (kind == 1 && d > 0) r2 = d + r1;           // touching inside (circle 1 inside circle 2)
        if (kind == 2 && d > r1) r2 = d - r1;          // touching outside
        if (kind == 3 && d > 0 && r1 > d) r2 = r1 - d;  // touching inside (circle 2 inside circle 1)
        auto res = circleCircle(c1, r1, c2, r2);
        size_t want;
        if (d < 1e-9) want = 0;
        else if (fabsl(d - (r1 + r2)) < 1e-9 || fabsl(d - fabsl(r1 - r2)) < 1e-9) want = 1;
        else if (d < r1 + r2 - 1e-6 && d > fabsl(r1 - r2) + 1e-6) want = 2;
        else if (d > r1 + r2 + 1e-6 || d < fabsl(r1 - r2) - 1e-6) want = 0;
        else continue;  // too close to call
        CHECK_EQ(res.size(), want);
        for (P p : res) CHECK_NEAR((p - c1).dist(), r1, 1e-9), CHECK_NEAR((p - c2).dist(), r2, 1e-9);
        if (want == 2) CHECK((res[0] - res[1]).dist() > 1e-9);
    }
}
