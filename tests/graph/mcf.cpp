#include "test.h"
#include "graph/mcf.cpp"
using ll = long long;

int main() {
    for (int it = 0; it < 500; it++) {
        int n = (int)test::rnd(2, 5), m = (int)test::rnd(0, 6);
        int s = 0, t = n - 1;
        MinCostFlow<ll, ll> mcf(n);
        vector<array<ll, 4>> edges;
        // cost = base + h[u] - h[v] with base >= 0: negative edges, but every cycle costs >= 0
        vector<ll> h(n);
        for (auto &x : h) x = test::rnd(-5, 5);
        for (int e = 0; e < m; e++) {
            int u = (int)test::rnd(0, n - 1), v = (int)test::rnd(0, n - 1);
            if (u == v) continue;
            ll cost = test::rnd(0, 9) + h[u] - h[v];
            ll cap = test::rnd(0, 2);
            mcf.addEdge(u, v, cap, cost);
            edges.push_back({u, v, cap, cost});
        }
        int E = (int)edges.size();
        // brute: every integral flow, keep (max value, min cost)
        ll bestF = 0, bestC = 0;
        vector<ll> x(E, 0);
        auto rec = [&](auto self, int i) -> void {
            if (i == E) {
                vector<ll> bal(n, 0);
                ll cost = 0;
                for (int e = 0; e < E; e++) bal[edges[e][0]] -= x[e], bal[edges[e][1]] += x[e], cost += x[e] * edges[e][3];
                for (int v = 0; v < n; v++)
                    if (v != s && v != t && bal[v]) return;
                if (bal[t] > bestF || (bal[t] == bestF && cost < bestC)) bestF = bal[t], bestC = cost;
                return;
            }
            for (x[i] = 0; x[i] <= edges[i][2]; x[i]++) self(self, i + 1);
            x[i] = 0;
        };
        rec(rec, 0);
        auto [f, c] = mcf.flow(s, t);
        CHECK_EQ(f, bestF);
        CHECK_EQ(c, bestC);
        ll cost = 0;
        for (int e = 0; e < E; e++) {
            ll fe = mcf.flowOn(e);
            CHECK(0 <= fe && fe <= edges[e][2]);
            cost += fe * edges[e][3];
        }
        CHECK_EQ(cost, c);
    }
    // flow limit: cheapest k units
    MinCostFlow mcf(4);
    mcf.addEdge(0, 1, 1, 1), mcf.addEdge(0, 2, 1, 5), mcf.addEdge(1, 3, 1, 1), mcf.addEdge(2, 3, 1, 1);
    CHECK_EQ(mcf.flow(0, 3, 1), (pair<ll, ll>(1, 2)));
}
