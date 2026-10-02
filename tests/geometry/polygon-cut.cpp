#include "test.h"
#include "geometry/point.cpp"
#include "geometry/convex-hull.cpp"
#include "geometry/point-in-polygon.cpp"
#include "geometry/polygon-area.cpp"
#include "geometry/polygon-cut.cpp"

using P = Point<long double>;

static P rp(int c) { return P(test::rnd(-c, c), test::rnd(-c, c)); }

int main() {
    // fixed cases: the unit square cut by x = 1/2, both sides
    vector<P> sq{{0, 0}, {1, 0}, {1, 1}, {0, 1}};
    auto left = polygonCut(sq, P{0.5L, 0}, P{0.5L, 1});  // keeps x <= 1/2
    CHECK_NEAR(polyArea2(left), 1, 1e-12);
    for (P p : left) CHECK(p.x <= 0.5L + 1e-12);
    auto right = polygonCut(sq, P{0.5L, 1}, P{0.5L, 0});
    CHECK_NEAR(polyArea2(right), 1, 1e-12);
    CHECK_EQ(polygonCut(sq, P{0, 0}, P{1, 0}).size(), 4u);   // the line is an edge: everything kept
    CHECK_EQ(polygonCut(sq, P{1, 0}, P{0, 0}).size(), 2u);   // only the edge on the line survives
    CHECK_EQ(polygonCut(sq, P{0, 5}, P{1, 5}).size(), 0u);   // all on the right
    CHECK_NEAR(polyArea2(polygonCut(sq, P{0, 0}, P{1, 1})), 1, 1e-12);  // along the diagonal
    CHECK_EQ(polygonCut(vector<P>{}, P{0, 0}, P{1, 1}).size(), 0u);
    CHECK_NEAR(polyArea2(polygonCut(vector<Point<double>>{{0, 0}, {2, 0}, {0, 2}}, {1, 0}, {1, 1})), 3, 1e-12);

    for (int it = 0; it < 3000; it++) {
        int c = it % 2 ? 4 : 1000;
        P a = rp(c + 2), b = rp(c + 2);
        if (a == b) continue;
        // any simple or self-touching vertex list: the two sides add up to the whole (signed) area
        vector<P> p(test::rnd(1, 10));
        for (auto &q : p) q = rp(c);
        auto l = polygonCut(p, a, b), r = polygonCut(p, b, a);
        CHECK_NEAR(polyArea2(l) + polyArea2(r), polyArea2(p), 1e-6);
        for (P q : l) CHECK(a.cross(b, q) >= -1e-6);
        for (P q : r) CHECK(a.cross(b, q) <= 1e-6);
        // convex polygons: the cut is the intersection with the half-plane
        vector<P> h;
        for (int i : convexHull(p)) h.push_back(p[i]);
        if (h.size() < 3) continue;
        auto cut = polygonCut(h, a, b);
        for (int k = 0; k < 30; k++) {
            P q(test::rndReal(-c, c), test::rndReal(-c, c));
            long double side = a.cross(b, q) / (b - a).dist();
            int in = inPolygon(h, q);
            if (fabsl(side) < 1e-6 || in == 0) continue;
            int want = side > 0 && in > 0 ? 1 : -1;
            int got = cut.size() < 3 ? -1 : inPolygon(cut, q);
            if (got == 0) continue;  // on the cut edge (q is near the line)
            CHECK_EQ(got, want);
        }
    }
}
