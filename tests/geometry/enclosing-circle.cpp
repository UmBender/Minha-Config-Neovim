#include "test.h"
#include "geometry/point.cpp"
#include "geometry/enclosing-circle.cpp"

using P = Point<long double>;

// smallest circle through 2 points (diameter) or 3 points (circumcircle) that holds every point
static long double naiveRadius(const vector<P> &p) {
    int n = (int)p.size();
    if (n == 1) return 0;
    long double best = 1e300L;
    auto consider = [&](P o, long double r) {
        for (P q : p)
            if ((q - o).dist() > r + 1e-9) return;
        best = min(best, r);
    };
    for (int i = 0; i < n; i++)
        for (int j = i + 1; j < n; j++) {
            consider((p[i] + p[j]) / 2, (p[i] - p[j]).dist() / 2);
            for (int k = j + 1; k < n; k++) {
                P b = p[j] - p[i], c = p[k] - p[i];
                long double d = 2 * b.cross(c);
                if (fabsl(d) < 1e-12) continue;
                P o = p[i] + (b * c.dist2() - c * b.dist2()).perp() / d;
                consider(o, (o - p[i]).dist());
            }
        }
    return best;
}

int main() {
    // fixed cases
    auto [o1, r1] = enclosingCircle(vector<P>{{3, 4}});
    CHECK_NEAR(o1.x, 3, 0), CHECK_NEAR(o1.y, 4, 0), CHECK_NEAR(r1, 0, 0);
    auto [o2, r2] = enclosingCircle(vector<P>{{0, 0}, {4, 0}, {2, 1}});  // diameter
    CHECK_NEAR(o2.x, 2, 1e-12), CHECK_NEAR(o2.y, 0, 1e-12), CHECK_NEAR(r2, 2, 1e-12);
    auto [o3, r3] = enclosingCircle(vector<P>{{0, 0}, {2, 0}, {1, 1.7320508075688772L}});  // equilateral
    CHECK_NEAR(o3.x, 1, 1e-12), CHECK_NEAR(r3, 2 / sqrtl(3), 1e-12);
    auto [o4, r4] = enclosingCircle(vector<P>{{1, 1}, {1, 1}, {1, 1}});
    CHECK_NEAR(r4, 0, 0), CHECK_NEAR(o4.x, 1, 0);
    auto [o5, r5] = enclosingCircle(vector<P>{{0, 0}, {1, 1}, {2, 2}, {3, 3}});  // collinear
    CHECK_NEAR(o5.x, 1.5, 1e-12), CHECK_NEAR(r5, sqrtl(18) / 2, 1e-12);
    auto [o6, r6] = enclosingCircle(vector<Point<double>>{{0, 0}, {0, 2}});
    CHECK_NEAR(o6.y, 1, 1e-12), CHECK_NEAR(r6, 1, 1e-12);

    for (int it = 0; it < 2000; it++) {
        int n = (int)test::rnd(1, 12), c = it % 2 ? 3 : 1000;
        vector<P> p(n);
        for (auto &q : p) q = P(test::rnd(-c, c), test::rnd(-c, c));
        auto [o, r] = enclosingCircle(p);
        CHECK_NEAR(r, naiveRadius(p), 1e-9);
        for (P q : p) CHECK((q - o).dist() <= r + 1e-9);
    }
    // big: O(n) expected
    vector<P> p(200000);
    for (auto &q : p) {
        long double a = test::rndReal(0, 2 * acosl(-1)), d = sqrtl(test::rndReal(0, 1));
        q = P(cosl(a), sinl(a)) * (d * 100);
    }
    p.push_back(P{100, 0}), p.push_back(P{-100, 0});
    auto [o, r] = enclosingCircle(p);
    CHECK_NEAR(r, 100, 1e-9), CHECK_NEAR(o.dist(), 0, 1e-7);
}
