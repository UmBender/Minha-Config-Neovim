// Title: Eulerian path
// Description: Eulerian path or cycle (directed or undirected) using every edge once, with edge ids.
// Usage:
//   auto res = eulerianPath(n, edges, directed, cycle);   // edges: vector<pair<int, int>>
//   if (res) { auto &[verts, ids] = *res; }   // verts: m + 1 vertices, ids[i] goes verts[i] -> verts[i + 1]
//   nullopt if there is none; cycle = true requires verts.front() == verts.back()
//   the start is chosen automatically (the odd / out-heavy vertex when there is one)
// Complexity: O(n + m).
// Verify: https://judge.yosupo.jp/problem/eulerian_trail_directed
optional<pair<vector<int>, vector<int>>> eulerianPath(int n, const vector<pair<int, int>> &edges, bool directed, bool cycle) {
    int m = (int)edges.size();
    vector<vector<pair<int, int>>> g(n);
    vector<int> bal(n, 0);  // directed: out - in; undirected: degree
    for (int i = 0; i < m; i++) {
        auto [u, v] = edges[i];
        g[u].push_back({v, i});
        if (directed) bal[u]++, bal[v]--;
        else g[v].push_back({u, i}), bal[u]++, bal[v]++;
    }
    int start = m ? edges[0].first : 0, odd = 0;
    for (int v = 0; v < n; v++) {
        bool bad = directed ? bal[v] != 0 : bal[v] % 2 != 0;
        if (!bad) continue;
        if (cycle || (directed && abs(bal[v]) > 1)) return nullopt;
        odd++;
        if (directed ? bal[v] == 1 : true) start = v;
    }
    if (odd > 2) return nullopt;
    // Hierholzer
    vector<int> ptr(n, 0), verts, ids;
    vector<char> used(m, 0);
    vector<pair<int, int>> st = {{start, -1}};
    while (!st.empty()) {
        int v = st.back().first;
        while (ptr[v] < (int)g[v].size() && used[g[v][ptr[v]].second]) ptr[v]++;
        if (ptr[v] == (int)g[v].size()) {
            verts.push_back(v), ids.push_back(st.back().second), st.pop_back();
            continue;
        }
        auto [to, id] = g[v][ptr[v]++];
        used[id] = 1, st.push_back({to, id});
    }
    if ((int)verts.size() != m + 1) return nullopt;  // edges in more than one component
    ids.pop_back();  // the -1 of the start, popped last
    reverse(verts.begin(), verts.end());
    reverse(ids.begin(), ids.end());
    return pair{verts, ids};
}
