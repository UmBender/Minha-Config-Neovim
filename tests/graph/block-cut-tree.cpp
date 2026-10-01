#include "test.h"
#include "graph/block-cut-tree.cpp"

// brute: edges on a common simple cycle are in the same block
int main() {
    for (int it = 0; it < 400; it++) {
        int n = (int)test::rnd(1, 7), m = (int)test::rnd(0, 9);
        vector<pair<int, int>> edges;
        vector<vector<int>> g(n);
        vector<vector<pair<int, int>>> adj(n);  // (to, edge id)
        for (int e = 0; e < m; e++) {
            int u = (int)test::rnd(0, n - 1), v = (int)test::rnd(0, n - 1);
            if (u == v) continue;
            int id = (int)edges.size();
            edges.push_back({u, v});
            g[u].push_back(v), g[v].push_back(u);
            adj[u].push_back({v, id}), adj[v].push_back({u, id});
        }
        int E = (int)edges.size();
        vector<int> p(E);
        iota(p.begin(), p.end(), 0);
        auto find = [&](auto self, int x) -> int { return p[x] == x ? x : p[x] = self(self, p[x]); };
        // enumerate simple cycles starting at their smallest vertex
        vector<int> pathEdges;
        vector<char> onPath(n, 0);
        auto dfs = [&](auto self, int start, int v, int lastEdge) -> void {
            for (auto [to, id] : adj[v]) {
                if (id == lastEdge) continue;
                if (to == start && !pathEdges.empty()) {
                    for (int x : pathEdges) p[find(find, x)] = find(find, id);
                } else if (!onPath[to] && to > start) {
                    onPath[to] = 1, pathEdges.push_back(id);
                    self(self, start, to, id);
                    onPath[to] = 0, pathEdges.pop_back();
                }
            }
        };
        for (int s = 0; s < n; s++) onPath[s] = 1, dfs(dfs, s, s, -1), onPath[s] = 0;
        set<vector<int>> want;
        map<int, set<int>> cls;
        for (int e = 0; e < E; e++) cls[find(find, e)].insert(edges[e].first), cls[find(find, e)].insert(edges[e].second);
        for (auto &[c, vs] : cls) want.insert(vector<int>(vs.begin(), vs.end()));
        for (int v = 0; v < n; v++)
            if (g[v].empty()) want.insert({v});

        BlockCutTree bct(g);
        set<vector<int>> got;
        for (auto b : bct.blocks) {
            sort(b.begin(), b.end());
            got.insert(b);
        }
        CHECK_EQ(got, want);
        CHECK_EQ(bct.blocks.size(), want.size());
        // cut vertices: in two or more blocks
        for (int v = 0; v < n; v++) {
            int cnt = 0;
            for (auto &b : want) cnt += binary_search(b.begin(), b.end(), v);
            CHECK_EQ((bool)bct.isCut[v], cnt >= 2);
        }
        // tree: vertex v -- block n + i when v is in block i
        CHECK_EQ((int)bct.tree.size(), n + (int)bct.blocks.size());
        for (int i = 0; i < (int)bct.blocks.size(); i++)
            for (int v : bct.blocks[i]) {
                CHECK(count(bct.tree[v].begin(), bct.tree[v].end(), n + i) == 1);
                CHECK(count(bct.tree[n + i].begin(), bct.tree[n + i].end(), v) == 1);
            }
    }
}
