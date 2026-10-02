#include "test.h"
#include "tree/tree-matching.cpp"

int main() {
    CHECK_EQ(treeMatching(vector<vector<int>>(1)), vector<int>{-1});
    CHECK(treeMatching({}).empty());
    for (int it = 0; it < 1500; it++) {
        int n = (int)test::rnd(1, 13);
        auto t = test::rndTree(n);
        // a forest: drop some edges
        vector<vector<int>> g(n);
        vector<pair<int, int>> edges;
        for (int v = 0; v < n; v++)
            for (int u : t[v])
                if (u < v && test::rnd(0, 3)) edges.push_back({u, v});
        for (auto [u, v] : edges) g[u].push_back(v), g[v].push_back(u);
        for (auto &adj : g) shuffle(adj.begin(), adj.end(), test::gen);
        // brute force: largest set of pairwise disjoint edges
        int m = (int)edges.size(), best = 0;
        for (int mask = 0; mask < (1 << m); mask++) {
            int used = 0, ok = 1;
            for (int e = 0; e < m && ok; e++)
                if (mask >> e & 1) {
                    int b = 1 << edges[e].first | 1 << edges[e].second;
                    ok = !(used & b), used |= b;
                }
            if (ok) best = max(best, __builtin_popcount(mask));
        }
        auto mate = treeMatching(g);
        CHECK_EQ((int)mate.size(), n);
        int matched = 0;
        for (int v = 0; v < n; v++) {
            if (mate[v] == -1) continue;
            matched++;
            CHECK_EQ(mate[mate[v]], v);
            CHECK(count(g[v].begin(), g[v].end(), mate[v]) == 1);
        }
        CHECK_EQ(matched, 2 * best);
    }
    // long path: no recursion
    int n = 200001;
    vector<vector<int>> g(n);
    for (int i = 0; i + 1 < n; i++) g[i].push_back(i + 1), g[i + 1].push_back(i);
    auto mate = treeMatching(g);
    CHECK_EQ(count(mate.begin(), mate.end(), -1), 1);
}
