// Title: General matching
// Description: Maximum matching in a general graph (Edmonds blossom), self-loops and multi-edges OK.
// Usage:
//   vector<int> mate = generalMatching(g);   // g: undirected vector<vector<int>> (both directions)
//   mate[v] = partner of v or -1; matching size = count of mate[v] != -1, divided by 2
// Complexity: O(V^3).
// Verify: https://judge.yosupo.jp/problem/general_matching
vector<int> generalMatching(const vector<vector<int>> &g) {
    int n = (int)g.size();
    vector<int> mate(n, -1), par(n), base(n), q;
    vector<char> used(n), blossom(n);
    auto lca = [&](int a, int b) {
        vector<char> seen(n, 0);
        while (true) {
            a = base[a], seen[a] = 1;
            if (mate[a] == -1) break;
            a = par[mate[a]];
        }
        while (true) {
            b = base[b];
            if (seen[b]) return b;
            b = par[mate[b]];
        }
    };
    auto markPath = [&](int v, int b, int child) {
        while (base[v] != b) {
            blossom[base[v]] = blossom[base[mate[v]]] = 1;
            par[v] = child, child = mate[v], v = par[mate[v]];
        }
    };
    auto findPath = [&](int root) {
        fill(used.begin(), used.end(), 0), fill(par.begin(), par.end(), -1);
        iota(base.begin(), base.end(), 0);
        used[root] = 1, q = {root};
        for (int qi = 0; qi < (int)q.size(); qi++) {
            int v = q[qi];
            for (int to : g[v]) {
                if (base[v] == base[to] || mate[v] == to) continue;
                if (to == root || (mate[to] != -1 && par[mate[to]] != -1)) {
                    int cur = lca(v, to);
                    fill(blossom.begin(), blossom.end(), 0);
                    markPath(v, cur, to), markPath(to, cur, v);
                    for (int i = 0; i < n; i++)
                        if (blossom[base[i]]) {
                            base[i] = cur;
                            if (!used[i]) used[i] = 1, q.push_back(i);
                        }
                } else if (par[to] == -1) {
                    par[to] = v;
                    if (mate[to] == -1) return to;
                    used[mate[to]] = 1, q.push_back(mate[to]);
                }
            }
        }
        return -1;
    };
    for (int v = 0; v < n; v++) {
        if (mate[v] != -1) continue;
        for (int to : g[v])  // greedy start
            if (to != v && mate[to] == -1) {
                mate[to] = v, mate[v] = to;
                break;
            }
    }
    for (int v = 0; v < n; v++) {
        if (mate[v] != -1) continue;
        int u = findPath(v);
        while (u != -1) {
            int pv = par[u], ppv = mate[pv];
            mate[u] = pv, mate[pv] = u, u = ppv;
        }
    }
    return mate;
}
