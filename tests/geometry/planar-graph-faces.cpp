#include "test.h"
#include "geometry/point.cpp"
#include "geometry/planar-graph-faces.cpp"

using P = Point<long long>;

static bool onSeg(P p, P a, P b) { return (b - a).cross(p - a) == 0 && (p - a).dot(p - b) <= 0; }

// closed segments ab and cd share a point other than a common endpoint
static bool bad(P a, P b, P c, P d) {
    P r = b - a, s = d - c;
    long long den = r.cross(s);
    if (den == 0) {
        if (r.cross(c - a) != 0) return false;
        // collinear: overlapping beyond a single shared endpoint
        int touch = (a == c || a == d) + (b == c || b == d);
        if (touch) return (c != a && c != b && onSeg(c, a, b)) || (d != a && d != b && onSeg(d, a, b)) ||
                          (a != c && a != d && onSeg(a, c, d)) || (b != c && b != d && onSeg(b, c, d));
        return onSeg(c, a, b) || onSeg(d, a, b) || onSeg(a, c, d) || onSeg(b, c, d);
    }
    if (a == c || a == d || b == c || b == d) return false;
    long long tn = (c - a).cross(s), un = (c - a).cross(r);
    if (den < 0) den = -den, tn = -tn, un = -un;
    return 0 <= tn && tn <= den && 0 <= un && un <= den;
}

// random straight-line planar graph: distinct points, edges added greedily when they cross nothing
// and pass through no other vertex
static pair<vector<P>, vector<vector<int>>> rndPlanar(int n, int c, int tries) {
    set<P> used;
    vector<P> p;
    while ((int)p.size() < n) {
        P q{test::rnd(-c, c), test::rnd(-c, c)};
        if (used.insert(q).second) p.push_back(q);
    }
    vector<vector<int>> g(n);
    vector<pair<int, int>> es;
    for (int t = 0; t < tries; t++) {
        int u = (int)test::rnd(0, n - 1), v = (int)test::rnd(0, n - 1);
        if (u == v || count(g[u].begin(), g[u].end(), v)) continue;
        bool ok = true;
        for (int w = 0; w < n && ok; w++) ok = w == u || w == v || !onSeg(p[w], p[u], p[v]);
        for (auto [x, y] : es) ok = ok && !bad(p[u], p[v], p[x], p[y]);
        if (ok) g[u].push_back(v), g[v].push_back(u), es.push_back({u, v});
    }
    return {p, g};
}

static long long area2(const vector<P> &p, const vector<int> &f) {
    long long s = 0;
    for (int i = 0; i < (int)f.size(); i++) s += p[f[i]].cross(p[f[(i + 1) % f.size()]]);
    return s;
}

static void check(const vector<P> &p, const vector<vector<int>> &g) {
    int n = (int)g.size();
    auto faces = planarFaces(p, g);
    // every directed edge u -> v is used by exactly one face
    map<pair<int, int>, int> used;
    for (auto &f : faces) {
        CHECK(!f.empty());
        for (int i = 0; i < (int)f.size(); i++) {
            int u = f[i], v = f[(i + 1) % f.size()];
            CHECK(count(g[u].begin(), g[u].end(), v));
            used[{u, v}]++;
        }
    }
    int edges = 0;
    for (int u = 0; u < n; u++) edges += (int)g[u].size();
    CHECK_EQ((int)used.size(), edges);
    for (auto [e, k] : used) CHECK_EQ(k, 1);
    // per component with edges: Euler's formula, exactly one outer face (area <= 0, the smallest),
    // inner faces CCW, and the face areas cancel out
    vector<int> comp(n, -1);
    int nc = 0;
    for (int s = 0; s < n; s++) {
        if (comp[s] != -1 || g[s].empty()) continue;
        vector<int> st{s};
        comp[s] = nc;
        while (!st.empty()) {
            int v = st.back();
            st.pop_back();
            for (int u : g[v])
                if (comp[u] == -1) comp[u] = nc, st.push_back(u);
        }
        nc++;
    }
    vector<int> V(nc), E(nc), F(nc), outer(nc);
    vector<long long> total(nc);
    for (int v = 0; v < n; v++)
        if (comp[v] != -1) V[comp[v]]++, E[comp[v]] += (int)g[v].size();
    for (auto &f : faces) {
        int k = comp[f[0]];
        for (int v : f) CHECK_EQ(comp[v], k);
        long long a = area2(p, f);
        F[k]++, total[k] += a, outer[k] += a <= 0;
    }
    for (int k = 0; k < nc; k++) {
        CHECK_EQ(F[k], E[k] / 2 - V[k] + 2);
        CHECK_EQ(total[k], 0LL);
        CHECK_EQ(outer[k], 1);
    }
}

int main() {
    // fixed cases
    CHECK_EQ(planarFaces(vector<P>{{0, 0}}, vector<vector<int>>{{}}).size(), 0u);
    auto seg = planarFaces(vector<P>{{0, 0}, {1, 0}}, vector<vector<int>>{{1}, {0}});
    CHECK_EQ(seg.size(), 1u), CHECK_EQ(seg[0].size(), 2u);
    // a square with a diagonal: two triangles (CCW) and the outer face (CW)
    vector<P> sq{{0, 0}, {1, 0}, {1, 1}, {0, 1}};
    vector<vector<int>> g{{1, 3, 2}, {0, 2}, {1, 3, 0}, {2, 0}};
    auto faces = planarFaces(sq, g);
    CHECK_EQ(faces.size(), 3u);
    vector<long long> areas;
    for (auto &f : faces) areas.push_back(area2(sq, f));
    sort(areas.begin(), areas.end());
    CHECK_EQ(areas, (vector<long long>{-2, 1, 1}));
    check(sq, g);
    using PD = Point<long double>;
    CHECK_EQ(planarFaces(vector<PD>{{0, 0}, {0.5L, 0}, {0, 0.5L}}, vector<vector<int>>{{1, 2}, {0, 2}, {0, 1}}).size(), 2u);

    for (int it = 0; it < 1500; it++) {
        int n = (int)test::rnd(1, 12), c = it % 2 ? 3 : 1000;
        n = min(n, (2 * c + 1) * (2 * c + 1));
        auto [pts, adj] = rndPlanar(n, c, (int)test::rnd(0, 3 * n));
        for (auto &l : adj) shuffle(l.begin(), l.end(), test::gen);
        check(pts, adj);
    }
}
