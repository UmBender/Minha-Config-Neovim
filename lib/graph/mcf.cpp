// Title: Min-cost flow
// Description: Min-cost max flow (successive shortest paths, Dijkstra with potentials), negative costs OK.
// Usage:
//   MinCostFlow<Cap, Cost> mcf(n);   // both long long by default (MinCostFlow mcf(n))
//   int id = mcf.addEdge(u, v, cap, cost);   // directed
//   auto [f, c] = mcf.flow(s, t);    // max flow with min cost; mcf.flow(s, t, k): at most k units
//   mcf.flowOn(id)
//   negative costs are fine as long as there is no negative cycle (Bellman-Ford sets the potentials)
// Complexity: O(F (V + E) log V) plus one O(V E) Bellman-Ford when some cost is negative.
// Verify: https://judge.yosupo.jp/problem/min_cost_b_flow
template <class Cap = long long, class Cost = long long> struct MinCostFlow {
    struct Edge {
        int to;
        Cap cap;
        Cost cost;
    };
    static constexpr Cost INF = numeric_limits<Cost>::max();
    int n;
    vector<Edge> e;
    vector<Cap> orig;
    vector<vector<int>> g;
    MinCostFlow(int n_) : n(n_), g(n_) {}
    int addEdge(int u, int v, Cap cap, Cost cost) {
        g[u].push_back((int)e.size()), e.push_back({v, cap, cost});
        g[v].push_back((int)e.size()), e.push_back({u, Cap{}, -cost});
        orig.push_back(cap);
        return (int)orig.size() - 1;
    }
    Cap flowOn(int id) const { return orig[id] - e[2 * id].cap; }
    pair<Cap, Cost> flow(int s, int t, Cap limit = numeric_limits<Cap>::max()) {
        vector<Cost> pot(n, 0), dist(n);
        vector<int> parEdge(n);
        // potentials: Bellman-Ford on the residual graph (needed only with negative costs)
        for (int round = 0; round < n; round++) {
            bool changed = false;
            for (int v = 0; v < n; v++)
                for (int id : g[v])
                    if (e[id].cap > 0 && pot[v] + e[id].cost < pot[e[id].to]) pot[e[id].to] = pot[v] + e[id].cost, changed = true;
            if (!changed) break;
        }
        Cap f{};
        Cost c{};
        while (f < limit) {
            fill(dist.begin(), dist.end(), INF);
            priority_queue<pair<Cost, int>, vector<pair<Cost, int>>, greater<>> pq;
            dist[s] = 0, pq.push({0, s});
            while (!pq.empty()) {
                auto [d, v] = pq.top();
                pq.pop();
                if (d != dist[v]) continue;
                for (int id : g[v]) {
                    int to = e[id].to;
                    Cost nd = d + e[id].cost + pot[v] - pot[to];
                    if (e[id].cap > 0 && nd < dist[to]) dist[to] = nd, parEdge[to] = id, pq.push({nd, to});
                }
            }
            if (dist[t] == INF) break;
            for (int v = 0; v < n; v++)
                if (dist[v] != INF) pot[v] += dist[v];
            Cap push = limit - f;
            for (int v = t; v != s; v = e[parEdge[v] ^ 1].to) push = min(push, e[parEdge[v]].cap);
            for (int v = t; v != s; v = e[parEdge[v] ^ 1].to) e[parEdge[v]].cap -= push, e[parEdge[v] ^ 1].cap += push;
            f += push, c += (Cost)push * (pot[t] - pot[s]);
        }
        return {f, c};
    }
};
