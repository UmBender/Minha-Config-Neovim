// Title: DSU (union-find)
// Description: Same code and API as the normal DSU, commented: how and why it works.
// Usage:
//   DSU dsu(n);
//   dsu.unite(a, b)   merges; false if already together
//   dsu.same(a, b);  dsu.find(x);  dsu.size(x) (size of x's set);  dsu.count() (number of sets)
//   dsu.groups()      vector of sets (each a vector of elements)
// Complexity: O(alpha(n)) amortized per operation.
//
// Idea: each set is a tree; its root is the representative. find(x) walks up to the root,
// unite(a, b) hangs one root under the other. Two tricks keep the trees almost flat:
//   union by size: hang the smaller tree under the bigger one, so a node's depth only grows when
//     its set at least doubles: depth <= log n;
//   path compression: after a find, point every node on the path straight at the root.
// Together: O(alpha(n)) amortized per operation (alpha: inverse Ackermann, < 5 in practice).
struct DSU {
    vector<int> p;  // p[x] < 0: x is a root and -p[x] is the size
    int comps;
    DSU(int n = 0) : p(n, -1), comps(n) {}
    int find(int x) {
        // first pass: find the root; second pass: path compression
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
        if (p[a] > p[b]) swap(a, b);  // sizes are negative: now a is the bigger set
        p[a] += p[b], p[b] = a, comps--;
        return true;
    }
    bool same(int a, int b) { return find(a) == find(b); }
    int size(int x) { return -p[find(x)]; }
    int count() const { return comps; }
    // bucket the elements by root; idx[r] = position of r's group in the result
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
