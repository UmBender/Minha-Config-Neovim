// Title: Tree diameter
// Description: Longest path of a tree (unweighted or with non-negative weights), as a vertex list.
// Usage:
//   vector<int> p = treeDiameter(g);          // g: vector<vector<int>>, diameter = p.size() - 1
//   auto [len, p] = treeDiameter(wg);         // wg: vector<vector<pair<int, T>>> {to, weight >= 0}
//   p goes from one end of the diameter to the other; the center is p[(p.size() - 1) / 2] (unweighted)
// Complexity: O(n).
// Verify: https://judge.yosupo.jp/problem/tree_diameter
template <class T> pair<T, vector<int>> treeDiameter(const vector<vector<pair<int, T>>> &g) {
    int n = (int)g.size();
    vector<T> d(n);
    vector<int> par(n);
    // farthest vertex from s (the farthest from any vertex is an end of a diameter)
    auto farthest = [&](int s) {
        vector<int> st{s};
        d[s] = T(), par[s] = -1;
        int best = s;
        while (!st.empty()) {
            int v = st.back();
            st.pop_back();
            if (d[v] > d[best]) best = v;
            for (auto [u, w] : g[v])
                if (u != par[v]) d[u] = d[v] + w, par[u] = v, st.push_back(u);
        }
        return best;
    };
    int a = farthest(farthest(0));
    vector<int> path;
    for (int v = a; v != -1; v = par[v]) path.push_back(v);
    return {d[a], path};
}

vector<int> treeDiameter(const vector<vector<int>> &g) {
    vector<vector<pair<int, int>>> wg(g.size());
    for (int v = 0; v < (int)g.size(); v++)
        for (int u : g[v]) wg[v].push_back({u, 1});
    return treeDiameter(wg).second;
}
