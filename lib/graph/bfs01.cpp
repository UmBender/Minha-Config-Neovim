// Title: 0-1 BFS
// Description: Shortest paths with weights 0/1 (deque); multi-source, path recovery.
// Usage:
//   BFS01 b(g, s);              // g: vector<vector<pair<int, T>>> with weights 0 or 1
//   BFS01 b(g, vector<int>{s1, s2});   // multi-source
//   b.dist[v] (numeric_limits<T>::max() if unreached); b.reached(v); b.par[v] (-1 at sources)
//   b.path(v)                   // source ... v, empty if unreached
//   Unweighted graphs: the bfs variant in the <leader>rl menu (plain BFS, same API).
// Complexity: O(n + m).
template <class T> struct BFS01 {
    static constexpr T INF = numeric_limits<T>::max();
    vector<T> dist;
    vector<int> par;
    BFS01(const vector<vector<pair<int, T>>> &g, int s) : BFS01(g, vector<int>{s}) {}
    BFS01(const vector<vector<pair<int, T>>> &g, const vector<int> &src) : dist(g.size(), INF), par(g.size(), -1) {
        deque<int> dq;
        for (int s : src) dist[s] = 0, dq.push_back(s);
        while (!dq.empty()) {
            int v = dq.front();
            dq.pop_front();
            for (auto [to, w] : g[v]) {
                if (dist[v] + w >= dist[to]) continue;
                dist[to] = dist[v] + w, par[to] = v;
                w == 0 ? dq.push_front(to) : dq.push_back(to);
            }
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

