#include "test.h"
#include "geometry/point.cpp"
#include "geometry/point-in-polygon.cpp"

using P = Point<long long>;

static bool naiveOn(P p, P a, P b) {
    return (b - a).cross(p - a) == 0 && min(a.x, b.x) <= p.x && p.x <= max(a.x, b.x) && min(a.y, b.y) <= p.y &&
           p.y <= max(a.y, b.y);
}

// -1 / 0 / 1 by the winding number (sum of signed angles), border checked first
static int naiveIn(const vector<P> &p, P q) {
    int n = (int)p.size();
    for (int i = 0; i < n; i++)
        if (naiveOn(q, p[i], p[(i + 1) % n])) return 0;
    long double w = 0;
    for (int i = 0; i < n; i++) {
        P a = p[i] - q, b = p[(i + 1) % n] - q;
        w += atan2l((long double)a.cross(b), (long double)a.dot(b));
    }
    return fabsl(w) > 1 ? 1 : -1;
}

static bool segsCross(P a, P b, P c, P d) {  // closed segments share a point (parametric)
    P r = b - a, t = d - c;
    long long den = r.cross(t);
    if (den == 0) {
        if (r.cross(c - a) != 0) return false;
        return naiveOn(c, a, b) || naiveOn(d, a, b) || naiveOn(a, c, d) || naiveOn(b, c, d);
    }
    long long tn = (c - a).cross(t), un = (c - a).cross(r);
    if (den < 0) den = -den, tn = -tn, un = -un;
    return 0 <= tn && tn <= den && 0 <= un && un <= den;
}

// a random simple polygon (rejection sampling on a small grid), any orientation
static vector<P> rndSimple(int n, int c) {
    while (true) {
        vector<P> p(n);
        for (auto &q : p) q = P{test::rnd(-c, c), test::rnd(-c, c)};
        bool ok = true;
        for (int i = 0; i < n && ok; i++)
            for (int j = i + 1; j < n && ok; j++) {
                P a = p[i], b = p[(i + 1) % n], cc = p[j], d = p[(j + 1) % n];
                if (j == i + 1 || (i == 0 && j == n - 1)) {  // neighbors: share only the common vertex
                    P common = j == i + 1 ? b : a, o1 = j == i + 1 ? a : b, o2 = j == i + 1 ? d : cc;
                    ok = (o1 - common).cross(o2 - common) != 0 || (o1 - common).dot(o2 - common) < 0;
                    ok = ok && a != b;
                } else {
                    ok = !segsCross(a, b, cc, d);
                }
            }
        if (ok) return p;
    }
}

// strictly convex CCW hull by brute force (edges with every other point to their left), plus, if
// `collinear`, the points lying on its edges
static vector<P> naiveHull(vector<P> pts, bool collinear) {
    sort(pts.begin(), pts.end());
    pts.erase(unique(pts.begin(), pts.end()), pts.end());
    int n = (int)pts.size();
    map<P, P> nxt;
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) {
            if (i == j) continue;
            bool ok = true;
            for (int k = 0; k < n && ok; k++) {
                long long cr = pts[i].cross(pts[j], pts[k]);
                ok = cr > 0 || (cr == 0 && (pts[k] - pts[i]).dot(pts[k] - pts[j]) <= 0);
            }
            if (ok) nxt[pts[i]] = pts[j];
        }
    if (nxt.empty()) return pts;
    vector<P> h{nxt.begin()->first};
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

int main() {
    // fixed cases: unit square
    vector<P> sq{{0, 0}, {2, 0}, {2, 2}, {0, 2}};
    for (auto f : {inConvex<long long>, inPolygon<long long>}) {
        CHECK_EQ(f(sq, P{1, 1}), 1), CHECK_EQ(f(sq, P{0, 0}), 0), CHECK_EQ(f(sq, P{1, 0}), 0);
        CHECK_EQ(f(sq, P{2, 1}), 0), CHECK_EQ(f(sq, P{0, 1}), 0), CHECK_EQ(f(sq, P{3, 0}), -1);
        CHECK_EQ(f(sq, P{-1, -1}), -1), CHECK_EQ(f(sq, P{1, 3}), -1), CHECK_EQ(f(sq, P{3, 2}), -1);
    }
    // degenerate convex polygons
    CHECK_EQ(inConvex(vector<P>{{1, 1}}, P{1, 1}), 0), CHECK_EQ(inConvex(vector<P>{{1, 1}}, P{1, 2}), -1);
    CHECK_EQ(inConvex(vector<P>{{0, 0}, {2, 2}}, P{1, 1}), 0), CHECK_EQ(inConvex(vector<P>{{0, 0}, {2, 2}}, P{3, 3}), -1);
    // non-convex: the ray from q passes through vertices
    vector<P> w{{0, 0}, {4, 0}, {4, 4}, {2, 2}, {0, 4}};
    CHECK_EQ(inPolygon(w, P{1, 2}), 1), CHECK_EQ(inPolygon(w, P{2, 3}), -1), CHECK_EQ(inPolygon(w, P{-1, 2}), -1);
    CHECK_EQ(inPolygon(w, P{3, 2}), 1), CHECK_EQ(inPolygon(w, P{2, 2}), 0), CHECK_EQ(inPolygon(w, P{-2, 0}), -1);
    // floating coordinates
    using PD = Point<long double>;
    vector<PD> tri{{0, 0}, {1, 0}, {0, 1}};
    CHECK_EQ(inConvex(tri, PD{0.2L, 0.2L}), 1), CHECK_EQ(inConvex(tri, PD{0.5L, 0.5L}), 0);
    CHECK_EQ(inPolygon(tri, PD{0.7L, 0.7L}), -1), CHECK_EQ(inPolygon(tri, PD{0.1L, 0.9L}), 0);

    // general simple polygons, both orientations
    for (int it = 0; it < 3000; it++) {
        int c = (int)test::rnd(2, 6);
        auto p = rndSimple((int)test::rnd(3, 8), c);
        for (int k = 0; k < 20; k++) {
            P q{test::rnd(-c - 1, c + 1), test::rnd(-c - 1, c + 1)};
            CHECK_EQ(inPolygon(p, q), naiveIn(p, q));
        }
    }
    // convex polygons (CCW), with or without collinear vertices on the edges
    for (int it = 0; it < 3000; it++) {
        int c = (int)test::rnd(1, it % 3 ? 4 : 1000);
        vector<P> pts(test::rnd(3, 15));
        for (auto &q : pts) q = P{test::rnd(-c, c), test::rnd(-c, c)};
        auto h = naiveHull(pts, it % 2);
        bool flat = true;
        for (P x : h) flat = flat && h.size() > 1 && h[0].cross(h[1], x) == 0;
        if (h.size() < 3 || flat) continue;  // inConvex needs a positive area
        int start = (int)test::rnd(0, (int)h.size() - 1);  // any starting vertex
        if (it % 2 == 0) rotate(h.begin(), h.begin() + start, h.end());
        for (int k = 0; k < 30; k++) {
            P q = k < 3 ? h[test::rnd(0, (int)h.size() - 1)] : P{test::rnd(-c - 1, c + 1), test::rnd(-c - 1, c + 1)};
            CHECK_EQ(inConvex(h, q), naiveIn(h, q));
        }
    }
}
