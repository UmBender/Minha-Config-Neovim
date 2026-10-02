#include "test.h"
#include "tree/lca.cpp"
#include "tree/virtual-tree.cpp"

int main() {
    for (int it = 0; it < 1500; it++) {
        int n = (int)test::rnd(1, 30), root = (int)test::rnd(0, n - 1);
        auto g = test::rndTree(n);
        LCA l(g, root);
        auto naiveLca = [&](int a, int b) {
            while (l.depth[a] > l.depth[b]) a = l.par[a];
            while (l.depth[b] > l.depth[a]) b = l.par[b];
            while (a != b) a = l.par[a], b = l.par[b];
            return a;
        };
        int k = (int)test::rnd(0, min(n, 8));
        vector<int> vs;
        for (int i = 0; i < k; i++) vs.push_back((int)test::rnd(0, n - 1));  // duplicates on purpose
        auto input = vs;
        auto [verts, edges] = virtualTree(l, vs);
        CHECK_EQ(vs, input);
        // vertex set: the given vertices and all their pairwise LCAs
        set<int> want(vs.begin(), vs.end());
        for (int a : vs)
            for (int b : vs) want.insert(naiveLca(a, b));
        CHECK_EQ(set<int>(verts.begin(), verts.end()), want);
        CHECK_EQ(verts.size(), want.size());
        for (int i = 0; i + 1 < (int)verts.size(); i++) CHECK(l.tin[verts[i]] < l.tin[verts[i + 1]]);
        // edges: each non-root vertex hangs from its nearest proper ancestor in the set
        CHECK_EQ(edges.size(), verts.empty() ? 0 : verts.size() - 1);
        for (int i = 1; i < (int)verts.size(); i++) {
            int c = verts[i], p = l.par[c];
            while (!want.count(p)) p = l.par[p];
            CHECK_EQ(edges[i - 1], make_pair(p, c));
        }
    }
    // long path: virtual tree of the two ends
    int n = 200000;
    vector<vector<int>> g(n);
    for (int i = 0; i + 1 < n; i++) g[i].push_back(i + 1), g[i + 1].push_back(i);
    LCA l(g, n / 2);
    auto [verts, edges] = virtualTree(l, {0, n - 1});
    CHECK_EQ(verts[0], n / 2);
    CHECK_EQ(set<int>(verts.begin(), verts.end()), (set<int>{0, n / 2, n - 1}));
    CHECK_EQ((set<pair<int, int>>(edges.begin(), edges.end())), (set<pair<int, int>>{{n / 2, 0}, {n / 2, n - 1}}));
}
