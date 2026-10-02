// Title: Tree matching
// Description: Maximum matching of a tree or forest, greedily matching leaves with their parents.
// Usage:
//   vector<int> mate = treeMatching(g);   // g: undirected adjacency list of a forest
//   mate[v]: partner of v, or -1 if v is unmatched; size = (n - count(mate, -1)) / 2
// Complexity: O(n).
// Verify: https://cses.fi/problemset/task/1130
vector<int> treeMatching(const vector<vector<int>> &g) {
    int n = (int)g.size();
    vector<int> par(n, -1), order, mate(n, -1);
    vector<char> seen(n, 0);
    for (int s = 0; s < n; s++) {
        if (seen[s]) continue;
        seen[s] = 1, order.push_back(s);
        for (int i = (int)order.size() - 1; i < (int)order.size(); i++)
            for (int u : g[order[i]])
                if (!seen[u]) seen[u] = 1, par[u] = order[i], order.push_back(u);
    }
    // children before parents: a free vertex with a free parent takes it
    for (int i = n - 1; i >= 0; i--) {
        int v = order[i], p = par[v];
        if (p != -1 && mate[v] == -1 && mate[p] == -1) mate[v] = p, mate[p] = v;
    }
    return mate;
}
