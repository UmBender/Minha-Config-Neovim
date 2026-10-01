// Title: DSU (union-find)
// Description: Disjoint set union with union by size and path compression; sizes, component count, groups.
// Usage:
//   DSU dsu(n);
//   dsu.unite(a, b)   merges; false if already together
//   dsu.same(a, b);  dsu.find(x);  dsu.size(x) (size of x's set);  dsu.count() (number of sets)
//   dsu.groups()      vector of sets (each a vector of elements)
// Complexity: O(alpha(n)) amortized per operation.
// Verify: https://judge.yosupo.jp/problem/unionfind
// Pending: example + presets (T-009..T-011), remove when done
struct DSU {
    vector<int> p;  // p[x] < 0: x is a root and -p[x] is the size
    int comps;
    DSU(int n = 0) : p(n, -1), comps(n) {}
    int find(int x) {
        int r = x;
        while (p[r] >= 0) r = p[r];
        while (p[x] >= 0) {
            int nxt = p[x];
            p[x] = r, x = nxt;
        }
        return r;
    }
    bool unite(int a, int b) {
        a = find(a), b = find(b);
        if (a == b) return false;
        if (p[a] > p[b]) swap(a, b);
        p[a] += p[b], p[b] = a, comps--;
        return true;
    }
    bool same(int a, int b) { return find(a) == find(b); }
    int size(int x) { return -p[find(x)]; }
    int count() const { return comps; }
    vector<vector<int>> groups() {
        int n = (int)p.size();
        vector<int> idx(n, -1);
        vector<vector<int>> res;
        for (int x = 0; x < n; x++) {
            int r = find(x);
            if (idx[r] < 0) idx[r] = (int)res.size(), res.emplace_back();
            res[idx[r]].push_back(x);
        }
        return res;
    }
};
