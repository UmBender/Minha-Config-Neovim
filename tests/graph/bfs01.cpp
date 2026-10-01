#include "test.h"
#include "graph/bfs01.cpp"

const int INF = INT_MAX;

vector<int> bellman(int n, const vector<array<int, 3>> &edges, const vector<int> &src) {
    vector<int> d(n, INF);
    for (int s : src) d[s] = 0;
    for (int round = 0; round < n; round++)
        for (auto [u, v, w] : edges)
            if (d[u] != INF && d[u] + w < d[v]) d[v] = d[u] + w;
    return d;
}

int main() {
    for (int it = 0; it < 400; it++) {
        int n = (int)test::rnd(1, 10), m = (int)test::rnd(0, 25);
        vector<vector<pair<int, int>>> g(n);
        vector<vector<int>> ug(n);
        vector<array<int, 3>> edges, uedges;
        for (int e = 0; e < m; e++) {
            int u = (int)test::rnd(0, n - 1), v = (int)test::rnd(0, n - 1), w = (int)test::rnd(0, 1);
            g[u].push_back({v, w}), edges.push_back({u, v, w});
            ug[u].push_back(v), uedges.push_back({u, v, 1});
        }
        vector<int> src = {(int)test::rnd(0, n - 1)};
        if (test::rnd(0, 1)) src.push_back((int)test::rnd(0, n - 1));
        auto want = bellman(n, edges, src), uwant = bellman(n, uedges, src);
        BFS01 b(g, src);
        BFS ub(ug, src);
        for (int v = 0; v < n; v++) {
            CHECK_EQ(b.dist[v], want[v]);
            CHECK_EQ(ub.dist[v], uwant[v]);
            CHECK_EQ(b.reached(v), want[v] != INF);
            if (!b.reached(v)) {
                CHECK(b.path(v).empty());
                continue;
            }
            // the path starts at a source, follows edges and has the right length
            auto p = b.path(v);
            CHECK(find(src.begin(), src.end(), p[0]) != src.end());
            CHECK_EQ(p.back(), v);
            int len = 0;
            for (size_t i = 0; i + 1 < p.size(); i++) {
                int best = INF;
                for (auto [to, w] : g[p[i]])
                    if (to == p[i + 1]) best = min(best, w);
                CHECK(best != INF);
                len += best;
            }
            CHECK_EQ(len, want[v]);
            auto up = ub.path(v);
            CHECK_EQ((int)up.size() - 1, uwant[v]);
        }
        BFS single(ug, src[0]);
        CHECK_EQ(single.dist[src[0]], 0);
    }
}
