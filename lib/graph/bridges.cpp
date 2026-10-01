// Title: Bridges and 2-edge-connected components
// Description: Bridges, 2-edge-connected components and the bridge tree, from an edge list.
// Usage:
//   TwoEdgeCC t(n, edges);      // edges: vector<pair<int, int>>, undirected, parallel edges/loops OK
//   t.isBridge[e]; t.bridges()  // edge ids
//   t.count; t.comp[v]          // 2-edge-connected component of v
//   t.tree()                    // bridge tree (forest): adjacency between components
// Complexity: O(n + m).
// Verify: https://judge.yosupo.jp/problem/two_edge_connected_components
struct TwoEdgeCC {
    int n, count = 0;
    vector<pair<int, int>> edges;
    vector<int> comp;
    vector<char> isBridge;
    TwoEdgeCC(int n_, const vector<pair<int, int>> &edges_)
        : n(n_), edges(edges_), comp(n_, -1), isBridge(edges_.size(), 0) {
        vector<vector<pair<int, int>>> g(n);
        for (int i = 0; i < (int)edges.size(); i++) {
            auto [a, b] = edges[i];
            g[a].push_back({b, i}), g[b].push_back({a, i});
        }
        vector<int> tin(n, -1), low(n), parEdge(n, -1), it(n, 0), st, call;
        int timer = 0;
        for (int s = 0; s < n; s++) {
            if (tin[s] != -1) continue;
            call.push_back(s), tin[s] = low[s] = timer++, st.push_back(s);
            while (!call.empty()) {
                int v = call.back();
                if (it[v] < (int)g[v].size()) {
                    auto [to, id] = g[v][it[v]++];
                    if (id == parEdge[v]) continue;
                    if (tin[to] == -1) {
                        tin[to] = low[to] = timer++, parEdge[to] = id;
                        st.push_back(to), call.push_back(to);
                    } else {
                        low[v] = min(low[v], tin[to]);
                    }
                    continue;
                }
                call.pop_back();
                if (call.empty() || low[v] == tin[v]) {
                    if (!call.empty()) isBridge[parEdge[v]] = 1;
                    int x;
                    do x = st.back(), st.pop_back(), comp[x] = count;
                    while (x != v);
                    count++;
                }
                if (!call.empty()) low[call.back()] = min(low[call.back()], low[v]);
            }
        }
    }
    vector<int> bridges() const {
        vector<int> res;
        for (int e = 0; e < (int)edges.size(); e++)
            if (isBridge[e]) res.push_back(e);
        return res;
    }
    vector<vector<int>> tree() const {
        vector<vector<int>> t(count);
        for (int e : bridges()) {
            int a = comp[edges[e].first], b = comp[edges[e].second];
            t[a].push_back(b), t[b].push_back(a);
        }
        return t;
    }
};
