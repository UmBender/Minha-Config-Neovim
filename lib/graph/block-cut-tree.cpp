// Title: Block-cut tree
// Description: Biconnected components (blocks), cut vertices and the block-cut tree (iterative).
// Usage:
//   BlockCutTree b(g);          // g: undirected vector<vector<int>> (both directions), no self-loops
//   b.isCut[v]                  // articulation point
//   b.blocks[i]                 // vertices of block i (an isolated vertex is its own block)
//   b.tree                      // n + blocks.size() nodes: vertex v -- block node n + i if v in block i
// Complexity: O(n + m).
// Verify: https://judge.yosupo.jp/problem/biconnected_components
struct BlockCutTree {
    int n;
    vector<char> isCut;
    vector<vector<int>> blocks, tree;
    BlockCutTree(const vector<vector<int>> &g) : n((int)g.size()), isCut(n, 0) {
        vector<int> tin(n, -1), low(n), it(n, 0), par(n, -1), st, call;
        int timer = 0;
        for (int s = 0; s < n; s++) {
            if (tin[s] != -1) continue;
            if (g[s].empty()) {
                blocks.push_back({s});
                tin[s] = timer++;
                continue;
            }
            int rootChildren = 0;
            call.push_back(s), tin[s] = low[s] = timer++, st.push_back(s);
            while (!call.empty()) {
                int v = call.back();
                if (it[v] < (int)g[v].size()) {
                    int to = g[v][it[v]++];
                    if (tin[to] == -1) {
                        tin[to] = low[to] = timer++, par[to] = v;
                        st.push_back(to), call.push_back(to);
                    } else {
                        low[v] = min(low[v], tin[to]);
                    }
                    continue;
                }
                call.pop_back();
                int p = par[v];
                if (p == -1) continue;
                low[p] = min(low[p], low[v]);
                if (low[v] >= tin[p]) {  // p separates v's subtree: pop a block
                    if (p == s) rootChildren++;
                    else isCut[p] = 1;
                    blocks.emplace_back();
                    int x;
                    do x = st.back(), st.pop_back(), blocks.back().push_back(x);
                    while (x != v);
                    blocks.back().push_back(p);
                }
            }
            if (rootChildren >= 2) isCut[s] = 1;
        }
        tree.assign(n + blocks.size(), {});
        for (int i = 0; i < (int)blocks.size(); i++)
            for (int v : blocks[i]) tree[v].push_back(n + i), tree[n + i].push_back(v);
    }
};
