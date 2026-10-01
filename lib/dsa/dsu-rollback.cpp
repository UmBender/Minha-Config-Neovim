// Title: DSU with rollback
// Description: Union-find that can undo unions (no path compression); snapshots via time().
// Usage:
//   RollbackDSU dsu(n);
//   dsu.unite(a, b)      false if already together (still recorded, so undo() always undoes one unite)
//   dsu.undo()           undo the last unite
//   int t = dsu.time();  ...  dsu.rollback(t);   // undo everything after the snapshot
//   dsu.same(a, b);  dsu.find(x);  dsu.size(x);  dsu.count()
//   Pairs with dsa/offline-deletions for offline dynamic connectivity.
// Complexity: O(log n) per operation.
// Pending: example + presets (T-009..T-011), remove when done
struct RollbackDSU {
    vector<int> p;  // p[x] < 0: root with size -p[x]
    vector<pair<int, int>> hist;  // (attached root, its old p), (-1, 0) for no-op unites
    int comps;
    RollbackDSU(int n = 0) : p(n, -1), comps(n) {}
    int find(int x) const {
        while (p[x] >= 0) x = p[x];
        return x;
    }
    bool unite(int a, int b) {
        a = find(a), b = find(b);
        if (a == b) {
            hist.push_back({-1, 0});
            return false;
        }
        if (p[a] > p[b]) swap(a, b);
        hist.push_back({b, p[b]});
        p[a] += p[b], p[b] = a, comps--;
        return true;
    }
    void undo() {
        auto [b, pb] = hist.back();
        hist.pop_back();
        if (b < 0) return;
        p[p[b]] -= pb, p[b] = pb, comps++;
    }
    int time() const { return (int)hist.size(); }
    void rollback(int t) {
        while ((int)hist.size() > t) undo();
    }
    bool same(int a, int b) const { return find(a) == find(b); }
    int size(int x) const { return -p[find(x)]; }
    int count() const { return comps; }
};
