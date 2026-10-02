#include "test.h"
#include "tree/hld.cpp"

static pair<vector<int>, vector<int>> rooted(const vector<vector<int>> &g, int root) {
    int n = (int)g.size();
    vector<int> par(n, -1), dep(n, 0), q{root};
    vector<char> seen(n, 0);
    seen[root] = 1;
    for (int i = 0; i < (int)q.size(); i++)
        for (int u : g[q[i]])
            if (!seen[u]) seen[u] = 1, par[u] = q[i], dep[u] = dep[q[i]] + 1, q.push_back(u);
    return {par, dep};
}

// vertices of the path u -> v, in order
static vector<int> pathOf(const vector<int> &par, const vector<int> &dep, int u, int v) {
    vector<int> a, b;
    while (dep[u] > dep[v]) a.push_back(u), u = par[u];
    while (dep[v] > dep[u]) b.push_back(v), v = par[v];
    while (u != v) a.push_back(u), b.push_back(v), u = par[u], v = par[v];
    a.push_back(u);
    a.insert(a.end(), b.rbegin(), b.rend());
    return a;
}

int main() {
    {
        HLD h(vector<vector<int>>(1));
        CHECK_EQ(h.lca(0, 0), 0);
        CHECK_EQ(h.segments(0, 0), (vector<pair<int, int>>{{0, 1}}));
        CHECK(h.segments(0, 0, true).empty());
        CHECK(h.path(0, 0, true).empty());
        CHECK_EQ(h.subtree(0), make_pair(0, 1));
        CHECK_EQ(h.kthAncestor(0, 1), -1);
    }
    for (int it = 0; it < 300; it++) {
        int n = (int)test::rnd(1, 24), root = (int)test::rnd(0, n - 1);
        auto g = test::rndTree(n);
        auto [par, dep] = rooted(g, root);
        HLD h(g, root);
        CHECK_EQ(h.par, par);
        CHECK_EQ(h.depth, dep);
        vector<int> at(n, -1), sz(n, 1);
        for (int v = 0; v < n; v++) CHECK(0 <= h.pos[v] && h.pos[v] < n && at[h.pos[v]] == -1), at[h.pos[v]] = v;
        // sizes by climbing
        for (int v = 0; v < n; v++)
            for (int p = par[v]; p != -1; p = par[p]) sz[p]++;
        CHECK_EQ(h.size, sz);
        for (int v = 0; v < n; v++) {
            // chains are contiguous and follow a child of maximum size
            if (h.head[v] != v) {
                CHECK(par[v] != -1 && h.head[v] == h.head[par[v]] && h.pos[v] == h.pos[par[v]] + 1);
                for (int u : g[par[v]])
                    if (u != par[par[v]]) CHECK(sz[u] <= sz[v]);
            } else {
                CHECK(v == root || h.head[par[v]] != v);
            }
            // subtree is a contiguous range
            auto [l, r] = h.subtree(v);
            CHECK_EQ(r - l, sz[v]);
            for (int x = l; x < r; x++) {
                int w = at[x];
                while (w != -1 && w != v) w = par[w];
                CHECK_EQ(w, v);
            }
            for (int k = 0; k <= dep[v] + 1; k++) {
                int w = v;
                for (int j = 0; j < k && w != -1; j++) w = par[w];
                CHECK_EQ(h.kthAncestor(v, k), w);
            }
        }
        int segLimit = 2 * (__lg(n) + 1);
        for (int u = 0; u < n; u++)
            for (int v = 0; v < n; v++) {
                auto want = pathOf(par, dep, u, v);
                int d = (int)want.size() - 1;
                CHECK_EQ(h.lca(u, v), *min_element(want.begin(), want.end(), [&](int a, int b) { return dep[a] < dep[b]; }));
                CHECK_EQ(h.dist(u, v), d);
                for (int k = -1; k <= d + 1; k++) CHECK_EQ(h.jump(u, v, k), 0 <= k && k <= d ? want[k] : -1);
                for (bool edges : {false, true}) {
                    auto wantV = want;
                    if (edges) wantV.erase(find(wantV.begin(), wantV.end(), h.lca(u, v)));
                    // unordered segments cover exactly the path vertices
                    vector<int> got;
                    auto segs = h.segments(u, v, edges);
                    CHECK((int)segs.size() <= segLimit);
                    for (auto [l, r] : segs) {
                        CHECK(l < r);
                        for (int x = l; x < r; x++) got.push_back(at[x]);
                    }
                    auto a = got, b = wantV;
                    sort(a.begin(), a.end()), sort(b.begin(), b.end());
                    CHECK_EQ(a, b);
                    // ordered segments spell the path from u to v
                    got.clear();
                    auto ps = h.path(u, v, edges);
                    CHECK((int)ps.size() <= segLimit);
                    for (auto [l, r, up] : ps) {
                        CHECK(l < r);
                        if (up)
                            for (int x = r - 1; x >= l; x--) got.push_back(at[x]);
                        else
                            for (int x = l; x < r; x++) got.push_back(at[x]);
                    }
                    CHECK_EQ(got, wantV);  // without the LCA: still u-side first, then v-side
                }
            }
    }
    // long path and a star: no recursion
    int n = 200000;
    vector<vector<int>> g(n), s(n);
    for (int i = 0; i + 1 < n; i++) g[i].push_back(i + 1), g[i + 1].push_back(i);
    for (int i = 1; i < n; i++) s[0].push_back(i), s[i].push_back(0);
    HLD a(g, n / 2);
    CHECK_EQ(a.lca(0, n - 1), n / 2);
    CHECK_EQ(a.jump(0, n - 1, n - 1), n - 1);
    CHECK_EQ((int)a.segments(0, n - 1).size(), 2);
    HLD b(s);
    CHECK_EQ(b.dist(1, n - 1), 2);
    CHECK((int)b.path(1, n - 1).size() <= 3);
}
