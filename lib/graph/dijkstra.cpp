// Title: Dijkstra
// Description: Single/multi-source shortest paths, non-negative weights of any type, path recovery.
// Usage:
//   Dijkstra d(g, s);           // g: vector<vector<pair<int, T>>>, T = long long, double, ...
//   Dijkstra d(g, vector<int>{s1, s2});   // multi-source
//   d.dist[v] (numeric_limits<T>::max() if unreached); d.reached(v); d.par[v] (-1 at sources)
//   d.path(v)                   // source ... v, empty if unreached
// Complexity: O((n + m) log m).
// Verify: https://judge.yosupo.jp/problem/shortest_path
template <class T> struct Dijkstra {
    static constexpr T INF = numeric_limits<T>::max();
    vector<T> dist;
    vector<int> par;
    Dijkstra(const vector<vector<pair<int, T>>> &g, int s) : Dijkstra(g, vector<int>{s}) {}
    Dijkstra(const vector<vector<pair<int, T>>> &g, const vector<int> &src) : dist(g.size(), INF), par(g.size(), -1) {
        priority_queue<pair<T, int>, vector<pair<T, int>>, greater<>> pq;
        for (int s : src) dist[s] = T{}, pq.push({T{}, s});
        while (!pq.empty()) {
            auto [d, v] = pq.top();
            pq.pop();
            if (d != dist[v]) continue;
            for (auto [to, w] : g[v])
                if (d + w < dist[to]) dist[to] = d + w, par[to] = v, pq.push({dist[to], to});
        }
    }
    bool reached(int v) const { return dist[v] != INF; }
    vector<int> path(int v) const {
        if (!reached(v)) return {};
        vector<int> p;
        for (; v != -1; v = par[v]) p.push_back(v);
        reverse(p.begin(), p.end());
        return p;
    }
};
