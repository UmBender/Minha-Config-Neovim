#include "test.h"
#include "graph/directed-mst.cpp"
using ll = long long;

int main() {
    for (int it = 0; it < 500; it++) {
        int n = (int)test::rnd(1, 6), m = (int)test::rnd(0, 12), root = (int)test::rnd(0, n - 1);
        vector<tuple<int, int, ll>> edges;
        for (int e = 0; e < m; e++)
            edges.push_back({(int)test::rnd(0, n - 1), (int)test::rnd(0, n - 1), test::rnd(-1e9, 1e9)});
        // brute: choose one incoming edge for every non-root vertex
        vector<vector<int>> in(n);
        for (int e = 0; e < m; e++) {
            auto [a, b, w] = edges[e];
            if (a != b && b != root) in[b].push_back(e);
        }
        ll best = LLONG_MAX;
        vector<int> pick(n, -1);
        auto rec = [&](auto self, int v) -> void {
            if (v == n) {
                ll s = 0;
                for (int u = 0; u < n; u++) {
                    if (u == root) continue;
                    s += get<2>(edges[pick[u]]);
                    int x = u, steps = 0;  // must reach the root
                    while (x != root && steps <= n) x = get<0>(edges[pick[x]]), steps++;
                    if (x != root) return;
                }
                best = min(best, s);
                return;
            }
            if (v == root) return self(self, v + 1);
            for (int e : in[v]) pick[v] = e, self(self, v + 1);
        };
        rec(rec, 0);
        auto res = directedMST(n, root, edges);
        CHECK_EQ(res.has_value(), best != LLONG_MAX);
        if (!res) continue;
        auto [cost, par] = *res;
        CHECK_EQ(cost, best);
        CHECK_EQ(par[root], -1);
        ll s = 0;
        for (int v = 0; v < n; v++) {
            if (v == root) continue;
            auto [a, b, w] = edges[par[v]];
            CHECK_EQ(b, v);
            s += w;
            int x = v, steps = 0;
            while (x != root && steps <= n) x = get<0>(edges[par[x]]), steps++;
            CHECK_EQ(x, root);
        }
        CHECK_EQ(s, cost);
    }
}
