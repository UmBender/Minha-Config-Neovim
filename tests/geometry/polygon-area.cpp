#include "test.h"
#include "geometry/point.cpp"
#include "geometry/polygon-area.cpp"

using P = Point<long long>;

int main() {
    CHECK_EQ(polyArea2(vector<P>{}), 0LL), CHECK_EQ(polyArea2(vector<P>{{1, 2}}), 0LL);
    CHECK_EQ(polyArea2(vector<P>{{1, 2}, {5, 7}}), 0LL);
    CHECK_EQ(polyArea2(vector<P>{{0, 0}, {4, 0}, {4, 3}, {0, 3}}), 24LL);   // CCW: positive
    CHECK_EQ(polyArea2(vector<P>{{0, 0}, {0, 3}, {4, 3}, {4, 0}}), -24LL);  // CW: negative
    CHECK_EQ(polyArea2(vector<P>{{0, 0}, {1, 0}, {0, 1}}), 1LL);            // area 1/2
    // non-convex: an L shape of area 3
    CHECK_EQ(polyArea2(vector<P>{{0, 0}, {2, 0}, {2, 1}, {1, 1}, {1, 2}, {0, 2}}), 6LL);
    CHECK_NEAR(polyArea2(vector<Point<long double>>{{0, 0}, {0.5L, 0}, {0, 0.5L}}), 0.25L, 1e-15);

    for (int it = 0; it < 5000; it++) {
        int n = (int)test::rnd(0, 12);
        vector<P> p(n);
        for (auto &q : p) q = P{test::rnd(-1e8, 1e8), test::rnd(-1e8, 1e8)};
        // fan from p[0], independent of the origin
        long long fan = 0;
        for (int i = 1; i + 1 < n; i++) fan += p[0].cross(p[i], p[i + 1]);
        CHECK_EQ(polyArea2(p), fan);
        // reversing the order flips the sign, a translation changes nothing, a rotation of the
        // vertex list changes nothing
        auto r = p;
        reverse(r.begin(), r.end());
        CHECK_EQ(polyArea2(r), -fan);
        P t{test::rnd(-1e8, 1e8), test::rnd(-1e8, 1e8)};
        for (auto &q : r) q = q + t;
        CHECK_EQ(polyArea2(r), -fan);
        if (n) rotate(p.begin(), p.begin() + test::rnd(0, n - 1), p.end());
        CHECK_EQ(polyArea2(p), fan);
    }
}
