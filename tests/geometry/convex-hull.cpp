#include "test.h"
#include "geometry/point.cpp"
#include "geometry/convex-hull.cpp"

using P = Point<long long>;

static bool naiveOn(P p, P a, P b) {
    return (b - a).cross(p - a) == 0 && (p - a).dot(p - b) <= 0;
}

// CCW hull from the smallest point, by brute force: an edge has every other point to its left (or
// on it); with `collinear`, the points on each edge are added in order
static vector<P> naiveHull(vector<P> pts, bool collinear) {
    sort(pts.begin(), pts.end());
    pts.erase(unique(pts.begin(), pts.end()), pts.end());
    int n = (int)pts.size();
    bool line = true;
    for (int i = 2; i < n; i++) line = line && pts[0].cross(pts[1], pts[i]) == 0;
    if (line) return collinear || n < 2 ? pts : vector<P>{pts[0], pts.back()};
    map<P, P> nxt;
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) {
            if (i == j) continue;
            bool ok = true;
            for (int k = 0; k < n && ok; k++) {
                long long cr = pts[i].cross(pts[j], pts[k]);
                ok = cr > 0 || (cr == 0 && naiveOn(pts[k], pts[i], pts[j]));
            }
            if (ok) nxt[pts[i]] = pts[j];
        }
    vector<P> h{pts[0]};
    while (nxt[h.back()] != h[0]) h.push_back(nxt[h.back()]);
    if (!collinear) return h;
    vector<P> res;
    for (int i = 0; i < (int)h.size(); i++) {
        P a = h[i], b = h[(i + 1) % h.size()];
        vector<P> on;
        for (P q : pts)
            if (q != b && naiveOn(q, a, b)) on.push_back(q);
        sort(on.begin(), on.end(), [&](P u, P v) { return (u - a).dist2() < (v - a).dist2(); });
        res.insert(res.end(), on.begin(), on.end());
    }
    return res;
}

static vector<P> pointsOf(const vector<P> &pts, const vector<int> &idx) {
    vector<P> r;
    for (int i : idx) CHECK(0 <= i && i < (int)pts.size()), r.push_back(pts[i]);
    return r;
}

int main() {
    // fixed cases
    CHECK_EQ(convexHull(vector<P>{}), vector<int>{});
    CHECK_EQ(convexHull(vector<P>{{3, 3}}), vector<int>{0});
    CHECK_EQ(convexHull(vector<P>{{3, 3}, {3, 3}, {3, 3}}).size(), 1u);
    vector<P> sq{{2, 2}, {0, 0}, {1, 1}, {2, 0}, {0, 2}, {1, 0}, {0, 0}};
    CHECK_EQ(convexHull(sq), (vector<int>{1, 3, 0, 4}));
    auto all = pointsOf(sq, convexHull(sq, true));
    CHECK_EQ(all, (vector<P>{{0, 0}, {1, 0}, {2, 0}, {2, 2}, {0, 2}}));
    vector<P> line{{4, 4}, {0, 0}, {2, 2}, {1, 1}};
    CHECK_EQ(convexHull(line), (vector<int>{1, 0}));
    CHECK_EQ(convexHull(line, true), (vector<int>{1, 3, 2, 0}));
    using PD = Point<long double>;
    CHECK_EQ(convexHull(vector<PD>{{0, 0}, {1, 0}, {0.5L, 0.5L}, {0, 1}, {0.2L, 0.2L}}), (vector<int>{0, 1, 3}));

    for (int it = 0; it < 4000; it++) {
        int c = (int)test::rnd(1, it % 3 ? 4 : 1000000000);
        int n = (int)test::rnd(1, 15);
        vector<P> pts(n);
        for (auto &q : pts) q = P{test::rnd(-c, c), test::rnd(-c, c)};
        if (it % 5 == 0)  // all on one line
            for (auto &q : pts) q = P{1, 2} * test::rnd(-4, 4);
        for (bool col : {false, true}) CHECK_EQ(pointsOf(pts, convexHull(pts, col)), naiveHull(pts, col));
    }
    // many duplicates (beyond insertion-sort sizes): each hull point is reported by its lowest index
    for (int it = 0; it < 50; it++) {
        vector<P> pts(200);
        for (auto &q : pts) q = P{test::rnd(-2, 2), test::rnd(-2, 2)};
        for (bool col : {false, true})
            for (int i : convexHull(pts, col))
                for (int j = 0; j < i; j++) CHECK(pts[j] != pts[i]);
    }
    // big: every point inside or on the hull, the hull strictly convex
    for (int it = 0; it < 5; it++) {
        int n = 3000, c = it < 3 ? 1000000000 : 30;
        vector<P> pts(n);
        for (auto &q : pts) q = P{test::rnd(-c, c), test::rnd(-c, c)};
        auto h = pointsOf(pts, convexHull(pts));
        int m = (int)h.size();
        for (int i = 0; i < m; i++) {
            CHECK(h[i].cross(h[(i + 1) % m], h[(i + 2) % m]) > 0);
            for (P q : pts) CHECK(h[i].cross(h[(i + 1) % m], q) >= 0);
        }
    }
}
