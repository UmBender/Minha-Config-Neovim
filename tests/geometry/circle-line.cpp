#include "test.h"
#include "geometry/point.cpp"
#include "geometry/circle-line.cpp"

using P = Point<long double>;

int main() {
    // fixed cases
    auto two = circleLine(P{0, 0}, 5.0L, P{-10, 3}, P{10, 3});
    CHECK_EQ(two.size(), 2u);
    CHECK_NEAR(two[0].x, -4, 1e-12), CHECK_NEAR(two[0].y, 3, 1e-12);  // in the direction a -> b
    CHECK_NEAR(two[1].x, 4, 1e-12), CHECK_NEAR(two[1].y, 3, 1e-12);
    auto one = circleLine(P{0, 0}, 5.0L, P{7, 5}, P{8, 5});  // tangent, the line only (not the segment)
    CHECK_EQ(one.size(), 1u), CHECK_NEAR(one[0].x, 0, 1e-9), CHECK_NEAR(one[0].y, 5, 1e-12);
    CHECK_EQ(circleLine(P{0, 0}, 5.0L, P{0, 6}, P{1, 6}).size(), 0u);
    CHECK_EQ(circleLine(Point<double>{1, 1}, 1.0, Point<double>{0, 0}, Point<double>{2, 2}).size(), 2u);

    for (int it = 0; it < 20000; it++) {
        auto rp = [] { return P(test::rnd(-100, 100), test::rnd(-100, 100)); };
        P c = rp(), a = rp(), b = rp();
        if (a == b) continue;
        long double dist = fabsl(a.cross(b, c)) / (b - a).dist();
        long double r = test::rnd(1, 120);
        if (it % 4 == 0) r = dist;  // tangent
        auto res = circleLine(c, r, a, b);
        size_t want = it % 4 == 0 ? 1 : dist < r - 1e-6 ? 2 : dist > r + 1e-6 ? 0 : 99;
        if (want == 99) continue;  // too close to call
        CHECK_EQ(res.size(), want);
        for (P p : res) {
            CHECK_NEAR((p - c).dist(), r, 1e-9);
            CHECK_NEAR(a.cross(b, p) / (b - a).dist(), 0, 1e-9);
        }
        if (res.size() == 2) CHECK((b - a).dot(res[1] - res[0]) > 0);
    }
}
