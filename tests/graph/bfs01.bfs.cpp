#include "test.h"
#include "graph/bfs01.bfs.cpp"

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
        vector<vector<int>> g(n);
        vector<array<int, 3>> edges;
        for (int e = 0; e < m; e++) {
            int u = (int)test::rnd(0, n - 1), v = (int)test::rnd(0, n - 1);
            g[u].push_back(v), edges.push_back({u, v, 1});
        }
        vector<int> src = {(int)test::rnd(0, n - 1)};
        if (test::rnd(0, 1)) src.push_back((int)test::rnd(0, n - 1));
        auto want = bellman(n, edges, src);
        BFS b(g, src);
        for (int v = 0; v < n; v++) {
            CHECK_EQ(b.dist[v], want[v]);
            CHECK_EQ(b.reached(v), want[v] != INF);
            auto p = b.path(v);
            if (!b.reached(v)) {
                CHECK(p.empty());
                continue;
            }
            CHECK(find(src.begin(), src.end(), p[0]) != src.end());
            CHECK_EQ(p.back(), v);
            CHECK_EQ((int)p.size() - 1, want[v]);
            for (size_t i = 0; i + 1 < p.size(); i++)
                CHECK(find(g[p[i]].begin(), g[p[i]].end(), p[i + 1]) != g[p[i]].end());
        }
        BFS single(g, src[0]);
        CHECK_EQ(single.dist[src[0]], 0);
        CHECK_EQ(single.par[src[0]], -1);
    }
}
