// Title: Hopcroft-Karp
// Description: Maximum bipartite matching and minimum vertex cover (Konig).
// Usage:
//   HopcroftKarp hk(nl, nr);    // left 0..nl-1, right 0..nr-1
//   hk.addEdge(u, v);           // u on the left, v on the right
//   int k = hk.maxMatching();   // hk.matchL[u] / hk.matchR[v], -1 if unmatched
//   auto [cl, cr] = hk.vertexCover();   // after maxMatching; |cl| + |cr| = k
//   max independent set = vertices not in the cover
// Complexity: O(E sqrt V).
// Verify: https://judge.yosupo.jp/problem/bipartite_matching
struct HopcroftKarp {
    int nl, nr;
    vector<vector<int>> g;
    vector<int> matchL, matchR, dist;
    HopcroftKarp(int nl_, int nr_) : nl(nl_), nr(nr_), g(nl_), matchL(nl_, -1), matchR(nr_, -1), dist(nl_) {}
    void addEdge(int u, int v) { g[u].push_back(v); }
    bool bfs() {
        vector<int> q;
        for (int u = 0; u < nl; u++) dist[u] = matchL[u] == -1 ? 0 : -1;
        for (int u = 0; u < nl; u++)
            if (matchL[u] == -1) q.push_back(u);
        bool found = false;
        for (int i = 0; i < (int)q.size(); i++)
            for (int v : g[q[i]]) {
                int w = matchR[v];
                if (w == -1) found = true;
                else if (dist[w] == -1) dist[w] = dist[q[i]] + 1, q.push_back(w);
            }
        return found;
    }
    bool dfs(int u) {
        for (int v : g[u]) {
            int w = matchR[v];
            if (w == -1 || (dist[w] == dist[u] + 1 && dfs(w))) {
                matchL[u] = v, matchR[v] = u;
                return true;
            }
        }
        dist[u] = -1;
        return false;
    }
    int maxMatching() {
        while (bfs())
            for (int u = 0; u < nl; u++)
                if (matchL[u] == -1) dfs(u);
        return (int)count_if(matchL.begin(), matchL.end(), [](int v) { return v != -1; });
    }
    pair<vector<int>, vector<int>> vertexCover() {
        // alternating paths from free left vertices; cover = unvisited left + visited right
        vector<char> visL(nl, 0), visR(nr, 0);
        vector<int> q;
        for (int u = 0; u < nl; u++)
            if (matchL[u] == -1) visL[u] = 1, q.push_back(u);
        for (int i = 0; i < (int)q.size(); i++)
            for (int v : g[q[i]])
                if (!visR[v]) {
                    visR[v] = 1;
                    int w = matchR[v];
                    if (w != -1 && !visL[w]) visL[w] = 1, q.push_back(w);
                }
        vector<int> cl, cr;
        for (int u = 0; u < nl; u++)
            if (!visL[u]) cl.push_back(u);
        for (int v = 0; v < nr; v++)
            if (visR[v]) cr.push_back(v);
        return {cl, cr};
    }
};
