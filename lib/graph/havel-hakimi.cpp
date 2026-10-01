// Title: Havel-Hakimi
// Description: Builds a simple graph with the given degree sequence, or reports that none exists.
// Usage:
//   auto res = havelHakimi(d);  // d: vector<int>, d[v] = wanted degree of v
//   if (res) for (auto [u, v] : *res) ...   // edges of a simple graph; nullopt if d isn't graphical
// Complexity: O((n + m) log n).
optional<vector<pair<int, int>>> havelHakimi(const vector<int> &d) {
    int n = (int)d.size();
    set<pair<int, int>> s;  // (remaining degree, vertex)
    for (int v = 0; v < n; v++) {
        if (d[v] < 0 || d[v] >= n) return nullopt;
        if (d[v]) s.insert({d[v], v});
    }
    vector<pair<int, int>> edges;
    while (!s.empty()) {
        // the vertex of largest remaining degree takes the next largest ones
        auto [k, u] = *s.rbegin();
        s.erase(prev(s.end()));
        if ((int)s.size() < k) return nullopt;
        vector<pair<int, int>> took;
        for (int i = 0; i < k; i++) {
            auto [dv, v] = *s.rbegin();
            s.erase(prev(s.end()));
            edges.push_back({u, v});
            if (dv > 1) took.push_back({dv - 1, v});
        }
        s.insert(took.begin(), took.end());
    }
    return edges;
}
