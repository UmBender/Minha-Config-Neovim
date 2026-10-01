// Title: BFS
// Description: Plain BFS on an unweighted graph; multi-source, path recovery.
// Usage:
//   BFS b(g, s);  BFS b(g, vector<int>{s1, s2});   // g: vector<vector<int>>
//   b.dist[v] (INT_MAX if unreached); b.reached(v); b.par[v] (-1 at sources)
//   b.path(v)                   // source ... v, empty if unreached
// Complexity: O(n + m).
struct BFS {
    static constexpr int INF = INT_MAX;
    vector<int> dist, par;
    BFS(const vector<vector<int>> &g, int s) : BFS(g, vector<int>{s}) {}
    BFS(const vector<vector<int>> &g, const vector<int> &src) : dist(g.size(), INF), par(g.size(), -1) {
        vector<int> q;
        for (int s : src)
            if (dist[s] != 0) dist[s] = 0, q.push_back(s);
        for (int i = 0; i < (int)q.size(); i++)
            for (int to : g[q[i]])
                if (dist[to] == INF) dist[to] = dist[q[i]] + 1, par[to] = q[i], q.push_back(to);
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
