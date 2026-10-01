#include "test.h"
#include "graph/hopcroft-karp.cpp"

int main() {
    for (int it = 0; it < 500; it++) {
        int nl = (int)test::rnd(0, 7), nr = (int)test::rnd(0, 7), m = nl && nr ? (int)test::rnd(0, 20) : 0;
        HopcroftKarp hk(nl, nr);
        vector<pair<int, int>> edges;
        vector<vector<char>> adj(nl, vector<char>(nr, 0));
        for (int e = 0; e < m; e++) {
            int u = (int)test::rnd(0, nl - 1), v = (int)test::rnd(0, nr - 1);
            hk.addEdge(u, v), edges.push_back({u, v}), adj[u][v] = 1;
        }
        // brute: best over left vertices in order, right set as a mask
        vector<map<int, int>> memo(nl + 1);
        auto best = [&](auto self, int i, int used) -> int {
            if (i == nl) return 0;
            auto itm = memo[i].find(used);
            if (itm != memo[i].end()) return itm->second;
            int r = self(self, i + 1, used);
            for (int v = 0; v < nr; v++)
                if (adj[i][v] && !(used >> v & 1)) r = max(r, 1 + self(self, i + 1, used | 1 << v));
            return memo[i][used] = r;
        };
        int want = best(best, 0, 0);
        CHECK_EQ(hk.maxMatching(), want);
        int cnt = 0;
        for (int u = 0; u < nl; u++)
            if (hk.matchL[u] != -1) {
                cnt++;
                CHECK(adj[u][hk.matchL[u]]);
                CHECK_EQ(hk.matchR[hk.matchL[u]], u);
            }
        CHECK_EQ(cnt, want);
        auto [cl, cr] = hk.vertexCover();
        CHECK_EQ((int)(cl.size() + cr.size()), want);
        set<int> sl(cl.begin(), cl.end()), sr(cr.begin(), cr.end());
        for (auto [u, v] : edges) CHECK(sl.count(u) || sr.count(v));
    }
}
