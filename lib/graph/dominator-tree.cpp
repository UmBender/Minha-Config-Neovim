// Title: Dominator tree
// Description: Immediate dominators of a directed graph from a source (Lengauer-Tarjan, iterative).
// Usage:
//   vector<int> idom = dominatorTree(g, s);   // g: directed vector<vector<int>>
//   idom[s] = s, idom[v] = -1 if v is unreachable from s
//   d dominates v (every path s -> v passes d) iff d is an ancestor of v in the tree idom
// Complexity: O(m log n).
// Verify: https://judge.yosupo.jp/problem/dominatortree
vector<int> dominatorTree(const vector<vector<int>> &g, int s) {
    int n = (int)g.size();
    vector<vector<int>> rg(n), bucket(n);
    for (int v = 0; v < n; v++)
        for (int to : g[v]) rg[to].push_back(v);
    // DFS preorder, iterative
    vector<int> tin(n, -1), order, par(n, -1), it(n, 0), st = {s};
    tin[s] = 0, order.push_back(s);
    while (!st.empty()) {
        int v = st.back();
        if (it[v] == (int)g[v].size()) {
            st.pop_back();
            continue;
        }
        int to = g[v][it[v]++];
        if (tin[to] == -1) tin[to] = (int)order.size(), order.push_back(to), par[to] = v, st.push_back(to);
    }
    int k = (int)order.size();
    // everything below works on preorder indices
    vector<int> sdom(k), dsu(k), best(k), idom(k), p(k);
    for (int i = 0; i < k; i++) sdom[i] = dsu[i] = best[i] = i, p[i] = i ? tin[par[order[i]]] : 0;
    auto find = [&](int v) {  // min sdom on the dsu path, with path compression
        vector<int> path;
        while (dsu[v] != dsu[dsu[v]]) path.push_back(v), v = dsu[v];
        for (int i = (int)path.size() - 1; i >= 0; i--) {
            int x = path[i];
            if (sdom[best[dsu[x]]] < sdom[best[x]]) best[x] = best[dsu[x]];
            dsu[x] = dsu[v];
        }
        return best[path.empty() ? v : path[0]];
    };
    auto eval = [&](int v) { return dsu[v] == v ? v : find(v); };
    for (int w = k - 1; w > 0; w--) {
        for (int pv : rg[order[w]]) {
            if (tin[pv] == -1) continue;
            sdom[w] = min(sdom[w], sdom[eval(tin[pv])]);
        }
        bucket[sdom[w]].push_back(w);
        int pw = p[w];
        dsu[w] = pw;
        for (int v : bucket[pw]) {
            int u = eval(v);
            idom[v] = sdom[u] < sdom[v] ? u : pw;
        }
        bucket[pw].clear();
    }
    for (int w = 1; w < k; w++)
        if (idom[w] != sdom[w]) idom[w] = idom[idom[w]];
    vector<int> res(n, -1);
    res[s] = s;
    for (int w = 1; w < k; w++) res[order[w]] = order[idom[w]];
    return res;
}
