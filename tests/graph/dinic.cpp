#include "test.h"
#include "graph/dinic.cpp"
using ll = long long;

int main() {
    for (int it = 0; it < 400; it++) {
        int n = (int)test::rnd(2, 8), m = (int)test::rnd(0, 16);
        int s = 0, t = n - 1;
        Dinic<ll> d(n);
        vector<array<ll, 3>> edges;
        for (int e = 0; e < m; e++) {
            int u = (int)test::rnd(0, n - 1), v = (int)test::rnd(0, n - 1);
            ll c = test::rnd(0, 1e12);
            CHECK_EQ(d.addEdge(u, v, c), e);
            edges.push_back({u, v, c});
        }
        // brute: min cut over all s-side sets
        ll best = LLONG_MAX;
        for (int mask = 0; mask < (1 << n); mask++) {
            if (!(mask >> s & 1) || (mask >> t & 1)) continue;
            ll cut = 0;
            for (auto [u, v, c] : edges)
                if ((mask >> u & 1) && !(mask >> v & 1)) cut += c;
            best = min(best, cut);
        }
        ll f = d.flow(s, t);
        CHECK_EQ(f, best);
        // edge flows: within capacity, conserved, value f
        vector<ll> bal(n, 0);
        for (int e = 0; e < m; e++) {
            ll x = d.flowOn(e);
            CHECK(0 <= x && x <= edges[e][2]);
            bal[edges[e][0]] -= x, bal[edges[e][1]] += x;
        }
        for (int v = 0; v < n; v++)
            if (v != s && v != t) CHECK_EQ(bal[v], 0LL);
        CHECK_EQ(bal[t], f);
        // min cut side has capacity f
        auto side = d.minCut();
        CHECK(side[s] && !side[t]);
        ll cut = 0;
        for (auto [u, v, c] : edges)
            if (side[u] && !side[v]) cut += c;
        CHECK_EQ(cut, f);
    }
    // the second augmenting path cancels flow on 1 -> 2 (needs the residual edges)
    {
        Dinic<int> c(8);
        for (auto [u, v] : vector<pair<int, int>>{{0, 1}, {1, 2}, {2, 3}, {0, 4}, {4, 5}, {5, 2}, {1, 6}, {6, 7}, {7, 3}})
            c.addEdge(u, v, 1);
        CHECK_EQ(c.flow(0, 3), 2);
    }
    // limit and default type
    Dinic d(3);
    d.addEdge(0, 1, 5), d.addEdge(1, 2, 5);
    CHECK_EQ(d.flow(0, 2, 3), 3LL);
    CHECK_EQ(d.flow(0, 2), 2LL);  // continues from the current flow
}
