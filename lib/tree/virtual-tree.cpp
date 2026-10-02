// Title: Virtual tree
// Description: Compressed tree of k chosen vertices and their pairwise LCAs, with (parent, child) edges.
// Usage:
//   LCA l(g, root);
//   auto [vs, edges] = virtualTree(l, chosen);   // chosen: any order, duplicates OK, not modified
//   vs: chosen vertices + pairwise LCAs in DFS order, vs[0] is the root (LCA of all); at most 2k - 1
//   edges[i] = {parent, vs[i + 1]}: parent is the nearest proper ancestor of vs[i + 1] in vs
//   edge length in the original tree: l.depth[child] - l.depth[parent]
//   typical use: DP over the queried vertices only (process vs backwards: children before parents)
// Complexity: O(k log k) per call (after the O(n log n) LCA build).
// Requires: tree/lca
// Verify: https://codeforces.com/contest/613/problem/D
pair<vector<int>, vector<pair<int, int>>> virtualTree(const LCA &l, vector<int> vs) {
    auto byTin = [&](int a, int b) { return l.tin[a] < l.tin[b]; };
    sort(vs.begin(), vs.end(), byTin);
    // the LCAs of DFS-order neighbors are all the pairwise LCAs
    for (int i = 0, k = (int)vs.size(); i + 1 < k; i++) vs.push_back(l.lca(vs[i], vs[i + 1]));
    sort(vs.begin(), vs.end(), byTin);
    vs.erase(unique(vs.begin(), vs.end()), vs.end());
    vector<pair<int, int>> edges;
    vector<int> st;  // the current root -> vertex chain
    for (int v : vs) {
        while (!st.empty() && !l.isAncestor(st.back(), v)) st.pop_back();
        if (!st.empty()) edges.push_back({st.back(), v});
        st.push_back(v);
    }
    return {vs, edges};
}
