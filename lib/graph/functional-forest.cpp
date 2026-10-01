// Title: Functional graph
// Description: Cycles, depths and k-th successor (k up to 1e18) in a functional graph v -> f[v].
// Usage:
//   FunctionalGraph fg(f);      // f: vector<int>, every vertex has exactly one outgoing edge
//   fg.onCycle[v]; fg.depth[v]  // steps until the cycle (0 on it)
//   fg.entry[v]                 // first vertex of the cycle reached from v
//   fg.cycleId[v]; fg.cycles[c] // vertices of cycle c in order (f[c[i]] = c[i + 1])
//   fg.pos[v]                   // index of v in its cycle (only for vertices on a cycle)
//   fg.jump(v, k)               // f applied k times
// Complexity: O(n log n) build, O(log n) per jump.
struct FunctionalGraph {
    int n, lg = 1;
    vector<int> f, depth, entry, cycleId, pos;
    vector<char> onCycle;
    vector<vector<int>> cycles, up;
    FunctionalGraph(const vector<int> &f_)
        : n((int)f_.size()), f(f_), depth(n, 0), entry(n, -1), cycleId(n, -1), pos(n, -1), onCycle(n, 0) {
        vector<int> state(n, 0);  // 0 new, 1 on the current walk, 2 done
        for (int s = 0; s < n; s++) {
            int x = s;
            while (state[x] == 0) state[x] = 1, x = f[x];
            if (state[x] == 1) {  // new cycle through x
                cycles.emplace_back();
                for (int y = x;; y = f[y]) {
                    onCycle[y] = 1, pos[y] = (int)cycles.back().size(), cycleId[y] = (int)cycles.size() - 1;
                    entry[y] = y, cycles.back().push_back(y);
                    if (f[y] == x) break;
                }
            }
            for (int y = s; state[y] == 1; y = f[y]) state[y] = 2;
        }
        // trees hanging from the cycles: BFS on the reverse edges
        vector<vector<int>> rg(n);
        for (int v = 0; v < n; v++)
            if (!onCycle[v]) rg[f[v]].push_back(v);
        vector<int> q;
        for (int v = 0; v < n; v++)
            if (onCycle[v]) q.push_back(v);
        for (int i = 0; i < (int)q.size(); i++)
            for (int c : rg[q[i]]) depth[c] = depth[q[i]] + 1, entry[c] = entry[q[i]], cycleId[c] = cycleId[q[i]], q.push_back(c);
        while ((1 << lg) < n) lg++;
        up.assign(lg, f);
        for (int j = 1; j < lg; j++)
            for (int v = 0; v < n; v++) up[j][v] = up[j - 1][up[j - 1][v]];
    }
    int jump(int v, long long k) const {
        if (k >= depth[v]) {
            k -= depth[v], v = entry[v];
            auto &c = cycles[cycleId[v]];
            return c[(pos[v] + k) % (long long)c.size()];
        }
        for (int j = 0; j < lg; j++)
            if (k >> j & 1) v = up[j][v];
        return v;
    }
};
