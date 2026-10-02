// Title: Centroid decomposition
// Description: Centroid tree of a tree or forest: parent and level of each vertex in it.
// Usage:
//   CentroidTree ct(g);          // g: undirected adjacency list of a forest
//   ct.par[v]                    // parent in the centroid tree, -1 for the centroid of a component
//   ct.level[v]                  // depth in the centroid tree (0 at the top), at most log2 n
//   every path u -> v of g goes through the centroid-tree LCA of u and v; typical use: for each
//   vertex, walk its <= log2 n + 1 centroid ancestors (c = v; c != -1; c = ct.par[c]),
//   or process ct.order (top-down: a centroid comes before the centroids below it)
// Complexity: O(n log n).
// Verify: https://atcoder.jp/contests/abc291/tasks/abc291_h
struct CentroidTree {
    vector<int> par, level, order;
    CentroidTree(const vector<vector<int>> &g) : par(g.size(), -1), level(g.size(), -1) {
        int n = (int)g.size();
        vector<int> sub(n), from(n), comp;
        // level == -1: not a centroid yet (still inside a component)
        for (int root = 0; root < n; root++) {
            if (level[root] != -1) continue;
            vector<pair<int, int>> todo{{root, -1}};  // {vertex of a component, centroid above it}
            for (int i = 0; i < (int)todo.size(); i++) {
                auto [s, up] = todo[i];
                comp.assign(1, s), from[s] = -1;
                for (int j = 0; j < (int)comp.size(); j++)
                    for (int u : g[comp[j]])
                        if (u != from[comp[j]] && level[u] == -1) from[u] = comp[j], comp.push_back(u);
                for (int j = (int)comp.size() - 1; j >= 0; j--) {
                    int v = comp[j];
                    sub[v] = 1;
                    for (int u : g[v])
                        if (u != from[v] && level[u] == -1) sub[v] += sub[u];
                }
                // walk towards the heavy side until no child subtree has more than half
                int m = (int)comp.size(), c = s;
                for (bool moved = true; moved;) {
                    moved = false;
                    for (int u : g[c])
                        if (u != from[c] && level[u] == -1 && 2 * sub[u] > m) {
                            c = u, moved = true;
                            break;
                        }
                }
                par[c] = up, level[c] = up == -1 ? 0 : level[up] + 1, order.push_back(c);
                for (int u : g[c])
                    if (level[u] == -1) todo.push_back({u, c});
            }
        }
    }
};
