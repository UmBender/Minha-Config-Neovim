// Title: Heavy-light decomposition
// Description: Splits tree paths into O(log n) ranges of a position array, for segment trees on paths.
// Usage:
//   HLD h(g, root);              // g: undirected adjacency list of a tree, root defaults to 0
//   seg.set(h.pos[v], val[v])    // vertex v lives at position pos[v] of the array
//   for (auto [l, r] : h.segments(u, v)) res = op(res, seg.query(l, r));   // commutative op
//   for (auto [l, r, up] : h.path(u, v)) res = op(res, up ? rseg.query(l, r) : seg.query(l, r));
//   path: ranges in order u -> v; up = read positions r-1 down to l (rseg combines in reverse)
//   edges = true (last argument) drops the LCA: edge values stored at the child vertex
//   h.subtree(v)                 // [l, r): positions of v's subtree
//   h.lca(u, v); h.dist(u, v); h.kthAncestor(v, k); h.jump(u, v, k)   // -1 when out of range
//   h.par[v], h.depth[v], h.size[v], h.head[v] (top of v's chain)
// Complexity: O(n) build, O(log n) per path (at most 2 log2 n + 2 ranges).
// Verify: https://judge.yosupo.jp/problem/vertex_set_path_composite
// Verify: https://judge.yosupo.jp/problem/vertex_add_path_sum
struct HLD {
    int n;
    vector<int> par, depth, size, head, pos, at;  // at[p]: vertex at position p
    HLD(const vector<vector<int>> &g, int root = 0)
        : n((int)g.size()), par(n, -1), depth(n), size(n, 1), head(n), pos(n), at(n) {
        vector<int> order{root}, heavy(n, -1);
        for (int i = 0; i < (int)order.size(); i++)
            for (int u : g[order[i]])
                if (u != par[order[i]]) par[u] = order[i], depth[u] = depth[order[i]] + 1, order.push_back(u);
        for (int i = n - 1; i > 0; i--) {
            int v = order[i], p = par[v];
            size[p] += size[v];
            if (heavy[p] == -1 || size[v] > size[heavy[p]]) heavy[p] = v;
        }
        // lay out a whole chain, then its light subtrees (LIFO): subtrees stay contiguous
        vector<int> st{root};
        for (int t = 0; !st.empty();) {
            int h = st.back();
            st.pop_back();
            for (int v = h; v != -1; v = heavy[v]) {
                head[v] = h, at[t] = v, pos[v] = t++;
                for (int u : g[v])
                    if (u != par[v] && u != heavy[v]) st.push_back(u);
            }
        }
    }
    pair<int, int> subtree(int v) const { return {pos[v], pos[v] + size[v]}; }
    int lca(int u, int v) const {
        for (; head[u] != head[v]; u = par[head[u]])
            if (depth[head[u]] < depth[head[v]]) swap(u, v);
        return depth[u] < depth[v] ? u : v;
    }
    int dist(int u, int v) const { return depth[u] + depth[v] - 2 * depth[lca(u, v)]; }
    int kthAncestor(int v, int k) const {
        if (k < 0 || k > depth[v]) return -1;
        while (depth[v] - depth[head[v]] < k) k -= depth[v] - depth[head[v]] + 1, v = par[head[v]];
        return at[pos[v] - k];
    }
    // k-th vertex on the path u -> v (0 is u)
    int jump(int u, int v, int k) const {
        int w = lca(u, v), du = depth[u] - depth[w], d = du + depth[v] - depth[w];
        if (k < 0 || k > d) return -1;
        return k <= du ? kthAncestor(u, k) : kthAncestor(v, d - k);
    }
    // {l, r, up}: positions [l, r), up = walked from r-1 down to l (towards the root)
    vector<tuple<int, int, bool>> path(int u, int v, bool edges = false) const {
        vector<tuple<int, int, bool>> a, b;  // u -> LCA (up), LCA -> v (down, reversed)
        while (head[u] != head[v]) {
            if (depth[head[u]] >= depth[head[v]]) a.emplace_back(pos[head[u]], pos[u] + 1, true), u = par[head[u]];
            else b.emplace_back(pos[head[v]], pos[v] + 1, false), v = par[head[v]];
        }
        if (depth[u] >= depth[v]) {
            if (pos[v] + edges <= pos[u]) a.emplace_back(pos[v] + edges, pos[u] + 1, true);
        } else if (pos[u] + edges <= pos[v]) {
            a.emplace_back(pos[u] + edges, pos[v] + 1, false);
        }
        a.insert(a.end(), b.rbegin(), b.rend());
        return a;
    }
    // the same ranges in no particular order, for commutative operations
    vector<pair<int, int>> segments(int u, int v, bool edges = false) const {
        vector<pair<int, int>> res;
        for (auto [l, r, up] : path(u, v, edges)) res.emplace_back(l, r);
        return res;
    }
};
