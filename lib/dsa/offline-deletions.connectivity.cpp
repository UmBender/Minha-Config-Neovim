// Title: Dynamic connectivity (offline)
// Description: Add/remove undirected edges and ask for components / connectivity, answered offline.
// Usage:
//   DynamicConnectivity dc(n);
//   dc.addEdge(a, b);  dc.removeEdge(a, b);  dc.query(a, b);   // in input order
//     removeEdge deletes one copy of the edge (either orientation); an absent edge is ignored
//   dc.run();                                                  // once, at the end
//   dc.comps[qi]       number of components at the qi-th query
//   dc.connected[qi]   1 if that query's a and b were connected
// Complexity: O((m + q) log q log n), m = number of added edges, q = number of queries.
struct DynamicConnectivity {
    int n, q = 0;
    map<pair<int, int>, vector<int>> open;  // edge -> query index at which each alive copy was added
    vector<array<int, 4>> spans;            // edge (a, b) alive during queries [l, r): {l, r, a, b}
    vector<pair<int, int>> ask;
    vector<int> comps, connected;
    DynamicConnectivity(int n_) : n(n_) {}
    void addEdge(int a, int b) { open[minmax(a, b)].push_back(q); }
    void removeEdge(int a, int b) {
        auto it = open.find(minmax(a, b));
        if (it == open.end()) return;
        spans.push_back({it->second.back(), q, a, b});
        it->second.pop_back();
        if (it->second.empty()) open.erase(it);
    }
    void query(int a, int b) { ask.push_back({a, b}), q++; }
    void run() {
        for (auto &[e, starts] : open)
            for (int l : starts) spans.push_back({l, q, e.first, e.second});
        open.clear();
        comps.assign(q, 0), connected.assign(q, 0);
        if (q == 0) return;
        // segment tree over query times: each edge goes to the O(log q) nodes covering its span
        vector<vector<pair<int, int>>> seg(4 * q);
        auto add = [&](auto self, int i, int l, int r, int ql, int qr, pair<int, int> e) -> void {
            if (qr <= l || r <= ql) return;
            if (ql <= l && r <= qr) return seg[i].push_back(e);
            int m = (l + r) / 2;
            self(self, 2 * i, l, m, ql, qr, e), self(self, 2 * i + 1, m, r, ql, qr, e);
        };
        for (auto [l, r, a, b] : spans)
            if (l < r) add(add, 1, 0, q, l, r, {a, b});
        // DSU with rollback: union by size, no path compression
        vector<int> p(n, -1);
        vector<pair<int, int>> hist;  // {root b hung under another, old p[b]}; b = -1: nothing happened
        int cnt = n;
        auto find = [&](int x) {
            while (p[x] >= 0) x = p[x];
            return x;
        };
        auto dfs = [&](auto self, int i, int l, int r) -> void {
            for (auto [a, b] : seg[i]) {
                a = find(a), b = find(b);
                if (a == b) {
                    hist.push_back({-1, 0});
                    continue;
                }
                if (p[a] > p[b]) swap(a, b);
                hist.push_back({b, p[b]});
                p[a] += p[b], p[b] = a, cnt--;
            }
            if (r - l == 1) comps[l] = cnt, connected[l] = find(ask[l].first) == find(ask[l].second);
            else {
                int m = (l + r) / 2;
                self(self, 2 * i, l, m), self(self, 2 * i + 1, m, r);
            }
            for (size_t k = 0; k < seg[i].size(); k++) {  // undo this node's unions, newest first
                auto [b, old] = hist.back();
                hist.pop_back();
                if (b < 0) continue;
                p[p[b]] -= old, p[b] = old, cnt++;
            }
        };
        dfs(dfs, 1, 0, q);
    }
};
