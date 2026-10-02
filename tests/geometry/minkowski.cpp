#include "test.h"
#include "geometry/point.cpp"
#include "geometry/convex-hull.cpp"
#include "geometry/minkowski.cpp"

using P = Point<long long>;

static vector<P> hullOf(const vector<P> &pts, bool collinear = false) {
    vector<P> h;
    for (int i : convexHull(pts, collinear)) h.push_back(pts[i]);
    return h;
}

// a random convex polygon in CCW order starting at any vertex
static vector<P> rndConvex(int c, bool collinear) {
    vector<P> pts(test::rnd(1, 10));
    for (auto &q : pts) q = P{test::rnd(-c, c), test::rnd(-c, c)};
    auto h = hullOf(pts, collinear);
    rotate(h.begin(), h.begin() + test::rnd(0, (int)h.size() - 1), h.end());
    return h;
}

int main() {
    // fixed cases
    vector<P> sq{{0, 0}, {1, 0}, {1, 1}, {0, 1}}, tri{{0, 0}, {2, 0}, {0, 2}};
    CHECK_EQ(minkowski(sq, tri), (vector<P>{{0, 0}, {3, 0}, {3, 1}, {1, 3}, {0, 3}}));
    CHECK_EQ(minkowski(sq, sq), (vector<P>{{0, 0}, {2, 0}, {2, 2}, {0, 2}}));  // collinear vertices removed
    CHECK_EQ(minkowski(vector<P>{{5, 5}}, tri), (vector<P>{{5, 5}, {7, 5}, {5, 7}}));
    CHECK_EQ(minkowski(vector<P>{{0, 0}, {1, 1}}, vector<P>{{0, 0}, {2, 2}}), (vector<P>{{0, 0}, {3, 3}}));
    CHECK_EQ(minkowski(vector<P>{{1, 0}}, vector<P>{{0, 1}}), (vector<P>{{1, 1}}));
    auto f = minkowski(vector<Point<long double>>{{0, 0}, {0.5L, 0}, {0, 0.5L}}, vector<Point<long double>>{{1, 1}});
    CHECK_EQ(f.size(), 3u), CHECK_NEAR(f[1].x, 1.5L, 1e-15);

    // against the hull of all the pairwise sums
    for (int it = 0; it < 5000; it++) {
        int c = (int)test::rnd(1, it % 2 ? 4 : 100000000);
        auto a = rndConvex(c, it % 3 == 0), b = rndConvex(c, it % 5 == 0);
        vector<P> sums;
        for (P x : a)
            for (P y : b) sums.push_back(x + y);
        CHECK_EQ(minkowski(a, b), hullOf(sums));
    }
}
