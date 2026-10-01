#include "test.h"
#include "graph/bridges.cpp"

int components(int n, const vector<pair<int, int>> &edges, int skip) {
    vector<int> p(n);
    iota(p.begin(), p.end(), 0);
    auto find = [&](auto self, int x) -> int { return p[x] == x ? x : p[x] = self(self, p[x]); };
    int c = n;
    for (int i = 0; i < (int)edges.size(); i++) {
        if (i == skip) continue;
        int a = find(find, edges[i].first), b = find(find, edges[i].second);
        if (a != b) p[a] = b, c--;
    }
    return c;
}

int main() {
    for (int it = 0; it < 500; it++) {
        int n = (int)test::rnd(1, 9), m = (int)test::rnd(0, 14);
        vector<pair<int, int>> edges;
        for (int e = 0; e < m; e++) edges.push_back({(int)test::rnd(0, n - 1), (int)test::rnd(0, n - 1)});
        TwoEdgeCC t(n, edges);
        int base = components(n, edges, -1);
        vector<pair<int, int>> nonBridges;
        for (int e = 0; e < m; e++) {
            bool want = components(n, edges, e) > base;
            CHECK_EQ((bool)t.isBridge[e], want);
            if (!want) nonBridges.push_back(edges[e]);
        }
        // 2-edge-connected components: connected without the bridges
        vector<int> p(n);
        iota(p.begin(), p.end(), 0);
        auto find = [&](auto self, int x) -> int { return p[x] == x ? x : p[x] = self(self, p[x]); };
        for (auto [a, b] : nonBridges) p[find(find, a)] = find(find, b);
        for (int u = 0; u < n; u++)
            for (int v = 0; v < n; v++) CHECK_EQ(t.comp[u] == t.comp[v], find(find, u) == find(find, v));
        CHECK_EQ(t.count, (int)set<int>(t.comp.begin(), t.comp.end()).size());
        vector<int> br = t.bridges();
        CHECK_EQ((int)br.size(), (int)count(t.isBridge.begin(), t.isBridge.end(), 1));
        // bridge tree: one edge per bridge between the two components
        auto tree = t.tree();
        CHECK_EQ((int)tree.size(), t.count);
        int deg = 0;
        for (auto &adj : tree) deg += (int)adj.size();
        CHECK_EQ(deg, 2 * (int)br.size());
    }
}
