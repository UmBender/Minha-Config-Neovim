// Title: st-numbering
// Description: Numbers the vertices 0..n-1, s first and t last, so every other vertex has a lower and a higher neighbor.
// Usage:
//   vector<int> num = stNumbering(g, s, t);   // g: undirected vector<vector<int>> (both directions), s != t
//   num[s] = 0, num[t] = n - 1; empty when G + edge {s, t} is not biconnected (no numbering exists)
// Complexity: O(n + m).
vector<int> stNumbering(const vector<vector<int>> &g, int s, int t) {
    int n = (int)g.size();
    // DFS from s that goes to t first (through the added edge s-t, id -2)
    vector<vector<pair<int, int>>> adj(n);
    adj[s].push_back({t, -2}), adj[t].push_back({s, -2});
    int id = 0;
    for (int v = 0; v < n; v++)
        for (int to : g[v])
            if (v < to) adj[v].push_back({to, id}), adj[to].push_back({v, id}), id++;
    vector<int> tin(n, -1), low(n), par(n, -1), parEdge(n, -1), it(n, 0), order, st = {s};
    tin[s] = 0, low[s] = s, order.push_back(s);
    while (!st.empty()) {
        int v = st.back();
        if (it[v] == (int)adj[v].size()) {
            st.pop_back();
            if (par[v] != -1 && tin[low[v]] < tin[low[par[v]]]) low[par[v]] = low[v];
            continue;
        }
        auto [to, e] = adj[v][it[v]++];
        if (e == parEdge[v]) continue;
        if (tin[to] == -1) {
            tin[to] = (int)order.size(), low[to] = to, par[to] = v, parEdge[to] = e;
            order.push_back(to), st.push_back(to);
        } else if (tin[to] < tin[low[v]]) {
            low[v] = to;
        }
    }
    // biconnected: connected, s has a single child (t) and no other vertex separates its subtree
    if ((int)order.size() != n) return {};
    for (int v = 0; v < n; v++)
        if (v != s && v != t && (par[v] == s || tin[low[v]] >= tin[par[v]])) return {};
    // Tarjan: insert each vertex next to its parent, on the side given by the sign of low
    vector<int> nxt(n, -1), prv(n, -1);
    vector<char> minus(n, 0);
    nxt[s] = t, prv[t] = s, minus[s] = 1;
    for (int v : order) {
        if (v == s || v == t) continue;
        int p = par[v];
        if (minus[low[v]]) {  // before p
            nxt[v] = p, prv[v] = prv[p];
            if (prv[p] != -1) nxt[prv[p]] = v;
            prv[p] = v, minus[p] = 0;
        } else {  // after p
            prv[v] = p, nxt[v] = nxt[p];
            if (nxt[p] != -1) prv[nxt[p]] = v;
            nxt[p] = v, minus[p] = 1;
        }
    }
    vector<int> num(n);
    int k = 0;
    for (int v = s; v != -1; v = nxt[v]) num[v] = k++;
    return num;
}
