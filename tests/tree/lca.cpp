#include "test.h"
#include "tree/lca.cpp"

// parent and depth of every vertex by a plain BFS from root
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

static bool ancestor(const vector<int> &par, int a, int v) {
    for (; v != -1; v = par[v])
        if (v == a) return true;
    return false;
}

int main() {
    // single vertex
    {
        LCA l(vector<vector<int>>(1));
        CHECK_EQ(l.lca(0, 0), 0);
        CHECK_EQ(l.dist(0, 0), 0);
        CHECK_EQ(l.par[0], -1);
        CHECK_EQ(l.tin[0], 0), CHECK_EQ(l.tout[0], 1);
    }
    for (int it = 0; it < 600; it++) {
        int n = (int)test::rnd(1, 30), root = (int)test::rnd(0, n - 1);
        auto g = test::rndTree(n);
        auto [par, dep] = rooted(g, root);
        LCA l(g, root);
        CHECK_EQ(l.par, par);
        CHECK_EQ(l.depth, dep);
        // tin is a permutation and [tin, tout) is exactly the subtree
        vector<int> seen(n, 0);
        for (int v = 0; v < n; v++) CHECK(0 <= l.tin[v] && l.tin[v] < n), seen[l.tin[v]]++;
        CHECK_EQ(seen, vector<int>(n, 1));
        for (int a = 0; a < n; a++)
            for (int v = 0; v < n; v++) {
                bool anc = ancestor(par, a, v);
                CHECK_EQ(l.isAncestor(a, v), anc);
                CHECK_EQ(l.tin[a] <= l.tin[v] && l.tin[v] < l.tout[a], anc);
                // naive LCA: climb the deeper one, then both
                int x = a, y = v;
                while (dep[x] > dep[y]) x = par[x];
                while (dep[y] > dep[x]) y = par[y];
                while (x != y) x = par[x], y = par[y];
                CHECK_EQ(l.lca(a, v), x);
                CHECK_EQ(l.dist(a, v), dep[a] + dep[v] - 2 * dep[x]);
            }
    }
    // long path: no recursion, rooted at an end and in the middle
    int n = 200000;
    vector<vector<int>> g(n);
    for (int i = 0; i + 1 < n; i++) g[i].push_back(i + 1), g[i + 1].push_back(i);
    LCA a(g);
    CHECK_EQ(a.lca(n - 1, n / 2), n / 2);
    CHECK_EQ(a.dist(0, n - 1), n - 1);
    LCA b(g, n / 2);
    CHECK_EQ(b.lca(0, n - 1), n / 2);
    CHECK_EQ(b.lca(0, 10), 10);
    CHECK_EQ(b.depth[0], n / 2);
}
