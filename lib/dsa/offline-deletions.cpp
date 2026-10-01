// Title: Offline deletions
// Description: Turn a structure with insert + undo-last into one with arbitrary deletions (offline).
// Usage:
//   OfflineDeletion<pair<int, int>> od;       // V needs operator< (stored in a map)
//   od.insert(x);  od.remove(x);  od.query();  // in input order; remove deletes one copy of x
//                                              (removing a value that isn't present is ignored)
//   od.run(ins, undo, answer);                 // call once, at the end
//     ins(x): insert x;  undo(): undo the most recent ins;  answer(qi): state for the qi-th query()
//   Dynamic connectivity (with dsa/dsu-rollback):
//     RollbackDSU dsu(n);
//     od.run([&](auto e) { dsu.unite(e.first, e.second); }, [&] { dsu.undo(); },
//            [&](int qi) { ans[qi] = dsu.count(); });
// Complexity: O(k log q) ins/undo calls, k = number of inserts, q = number of queries.
template <class V> struct OfflineDeletion {
    int q = 0;
    map<V, vector<int>> open;  // value -> query index at which each alive copy was inserted
    vector<tuple<int, int, V>> spans;  // alive during queries [l, r)
    void insert(const V &x) { open[x].push_back(q); }
    void remove(const V &x) {
        auto it = open.find(x);
        if (it == open.end()) return;
        spans.emplace_back(it->second.back(), q, x);
        it->second.pop_back();
        if (it->second.empty()) open.erase(it);
    }
    int query() { return q++; }
    template <class Ins, class Undo, class Ans> void run(Ins ins, Undo undo, Ans answer) {
        for (auto &[x, starts] : open)
            for (int l : starts) spans.emplace_back(l, q, x);
        open.clear();
        if (q == 0) return;
        vector<vector<V>> seg(4 * q);
        auto add = [&](auto self, int i, int l, int r, int ql, int qr, const V &x) -> void {
            if (qr <= l || r <= ql) return;
            if (ql <= l && r <= qr) {
                seg[i].push_back(x);
                return;
            }
            int m = (l + r) / 2;
            self(self, 2 * i, l, m, ql, qr, x), self(self, 2 * i + 1, m, r, ql, qr, x);
        };
        for (auto &[l, r, x] : spans)
            if (l < r) add(add, 1, 0, q, l, r, x);
        auto dfs = [&](auto self, int i, int l, int r) -> void {
            for (auto &x : seg[i]) ins(x);
            if (r - l == 1) answer(l);
            else {
                int m = (l + r) / 2;
                self(self, 2 * i, l, m), self(self, 2 * i + 1, m, r);
            }
            for (size_t k = 0; k < seg[i].size(); k++) undo();
        };
        dfs(dfs, 1, 0, q);
    }
};
