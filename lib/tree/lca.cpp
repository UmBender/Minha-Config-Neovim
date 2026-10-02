// Title: Lowest common ancestor
// Description: O(1) LCA by DFS order + sparse table, with depths, parents and subtree ranges.
// Usage:
//   LCA l(g, root);              // g: undirected adjacency list of a tree, root defaults to 0
//   l.lca(u, v); l.dist(u, v)    // dist in edges
//   l.par[v] (-1 at the root), l.depth[v]
//   l.tin[v], l.tout[v]          // preorder index; subtree of v = vertices with tin in [tin[v], tout[v])
//   l.isAncestor(a, v)           // a is on the path root -> v (a vertex is its own ancestor)
// Complexity: O(n log n) build, O(1) query.
// Verify: https://judge.yosupo.jp/problem/lca
struct LCA {
    int n;
    vector<int> par, depth, tin, tout, at;  // at[i]: vertex with tin i
    vector<vector<int>> t;                   // t[k][i]: min tin of par over positions [i, i + 2^k)
    LCA(const vector<vector<int>> &g, int root = 0)
        : n((int)g.size()), par(n, -1), depth(n), tin(n), tout(n) {
        vector<int> st{root};
        while (!st.empty()) {
            int v = st.back();
            st.pop_back();
            tin[v] = (int)at.size(), at.push_back(v);
            for (int u : g[v])
                if (u != par[v]) par[u] = v, depth[u] = depth[v] + 1, st.push_back(u);
        }
        for (int i = n - 1; i >= 0; i--) {
            int v = at[i];
            tout[v] = max(tout[v], i + 1);
            if (par[v] != -1) tout[par[v]] = max(tout[par[v]], tout[v]);
        }
        t.assign(1, vector<int>(n));
        for (int i = 0; i < n; i++) t[0][i] = i ? tin[par[at[i]]] : -1;
        for (int k = 1; (1 << k) <= n; k++) {
            t.emplace_back(n - (1 << k) + 1);
            for (int i = 0; i + (1 << k) <= n; i++) t[k][i] = min(t[k - 1][i], t[k - 1][i + (1 << (k - 1))]);
        }
    }
    // for tin[u] < tin[v], the LCA is the parent of the shallowest vertex at positions (tin[u], tin[v]]
    int lca(int u, int v) const {
        if (u == v) return u;
        int l = min(tin[u], tin[v]) + 1, r = max(tin[u], tin[v]) + 1, k = __lg(r - l);
        return at[min(t[k][l], t[k][r - (1 << k)])];
    }
    int dist(int u, int v) const { return depth[u] + depth[v] - 2 * depth[lca(u, v)]; }
    bool isAncestor(int a, int v) const { return tin[a] <= tin[v] && tin[v] < tout[a]; }
};
