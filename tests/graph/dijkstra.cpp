#include "test.h"
#include "graph/dijkstra.cpp"
using ll = long long;

int main() {
    for (int it = 0; it < 400; it++) {
        int n = (int)test::rnd(1, 10), m = (int)test::rnd(0, 30);
        vector<vector<pair<int, ll>>> g(n);
        vector<array<ll, 3>> edges;
        for (int e = 0; e < m; e++) {
            int u = (int)test::rnd(0, n - 1), v = (int)test::rnd(0, n - 1);
            ll w = test::rnd(0, 1e12);
            g[u].push_back({v, w}), edges.push_back({u, v, w});
        }
        vector<int> src = {(int)test::rnd(0, n - 1)};
        if (test::rnd(0, 2) == 0) src.push_back((int)test::rnd(0, n - 1));
        const ll INF = LLONG_MAX;
        vector<ll> want(n, INF);
        for (int s : src) want[s] = 0;
        for (int round = 0; round < n; round++)
            for (auto [u, v, w] : edges)
                if (want[u] != INF && want[u] + w < want[v]) want[v] = want[u] + w;
        Dijkstra d(g, src);
        for (int v = 0; v < n; v++) {
            CHECK_EQ(d.dist[v], want[v]);
            CHECK_EQ(d.reached(v), want[v] != INF);
            if (!d.reached(v)) continue;
            auto p = d.path(v);
            CHECK(find(src.begin(), src.end(), p[0]) != src.end());
            CHECK_EQ(p.back(), v);
            ll len = 0;
            for (size_t i = 0; i + 1 < p.size(); i++) {
                ll best = INF;
                for (auto [to, w] : g[p[i]])
                    if (to == p[i + 1]) best = min(best, w);
                CHECK(best != INF);
                len += best;
            }
            CHECK_EQ(len, want[v]);
        }
    }
    // doubles and the single-source constructor
    vector<vector<pair<int, double>>> g(3);
    g[0].push_back({1, 0.5}), g[1].push_back({2, 0.25}), g[0].push_back({2, 1.0});
    Dijkstra d(g, 0);
    CHECK_NEAR(d.dist[2], 0.75, 1e-12);
    CHECK_EQ(d.path(2), (vector<int>{0, 1, 2}));
}
