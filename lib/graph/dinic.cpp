// Title: Dinic max flow
// Description: Dinic max flow with edge ids, per-edge flow, flow limit and min cut.
// Usage:
//   Dinic<T> d(n);              // T = long long by default (Dinic d(n))
//   int id = d.addEdge(u, v, cap);   // directed; for undirected add both directions
//   T f = d.flow(s, t);         // or d.flow(s, t, limit); calls continue from the current flow
//   d.flowOn(id)                // flow on edge id
//   d.minCut()                  // side[v] = v reachable from s in the residual graph (after flow)
// Complexity: O(V^2 E) in general, O(E sqrt V) for unit capacities (bipartite matching).
// Verify: https://judge.yosupo.jp/problem/bipartite_matching
template <class T = long long> struct Dinic {
    struct Edge {
        int to;
        T cap;
    };
    int n, src = 0;
    vector<Edge> e;  // edge 2i is the i-th added edge, 2i + 1 its residual
    vector<T> orig;
    vector<vector<int>> g;
    vector<int> level, it;
    Dinic(int n_) : n(n_), g(n_), level(n_), it(n_) {}
    int addEdge(int u, int v, T cap) {
        g[u].push_back((int)e.size()), e.push_back({v, cap});
        g[v].push_back((int)e.size()), e.push_back({u, T{}});
        orig.push_back(cap);
        return (int)orig.size() - 1;
    }
    bool bfs(int s, int t) {
        fill(level.begin(), level.end(), -1);
        vector<int> q = {s};
        level[s] = 0;
        for (int i = 0; i < (int)q.size(); i++)
            for (int id : g[q[i]])
                if (e[id].cap > 0 && level[e[id].to] == -1) level[e[id].to] = level[q[i]] + 1, q.push_back(e[id].to);
        return level[t] != -1;
    }
    T dfs(int v, int t, T pushed) {
        if (v == t) return pushed;
        for (int &i = it[v]; i < (int)g[v].size(); i++) {
            int id = g[v][i], to = e[id].to;
            if (e[id].cap <= 0 || level[to] != level[v] + 1) continue;
            T got = dfs(to, t, min(pushed, e[id].cap));
            if (got > 0) {
                e[id].cap -= got, e[id ^ 1].cap += got;
                return got;
            }
        }
        return T{};
    }
    T flow(int s, int t, T limit = numeric_limits<T>::max()) {
        src = s;
        T total{};
        while (total < limit && bfs(s, t)) {
            fill(it.begin(), it.end(), 0);
            while (total < limit) {
                T got = dfs(s, t, limit - total);
                if (got <= 0) break;
                total += got;
            }
        }
        return total;
    }
    T flowOn(int id) const { return orig[id] - e[2 * id].cap; }
    vector<char> minCut() {
        bfs(src, src);
        vector<char> side(n);
        for (int v = 0; v < n; v++) side[v] = level[v] != -1;
        return side;
    }
};
