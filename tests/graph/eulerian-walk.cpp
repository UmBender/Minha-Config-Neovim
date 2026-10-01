#include "test.h"
#include "graph/eulerian-walk.cpp"

// existence by the classic conditions (edges in one component + degrees)
bool exists(int n, const vector<pair<int, int>> &edges, bool directed, bool cycle) {
    if (edges.empty()) return true;
    vector<int> p(n);
    iota(p.begin(), p.end(), 0);
    auto find = [&](auto self, int x) -> int { return p[x] == x ? x : p[x] = self(self, p[x]); };
    vector<int> out(n, 0), in(n, 0), deg(n, 0);
    for (auto [u, v] : edges) {
        p[find(find, u)] = find(find, v);
        out[u]++, in[v]++, deg[u]++, deg[v]++;
    }
    int root = find(find, edges[0].first);
    for (int v = 0; v < n; v++)
        if (deg[v] && find(find, v) != root) return false;
    if (directed) {
        int plus = 0, minus = 0;
        for (int v = 0; v < n; v++) {
            int d = out[v] - in[v];
            if (d == 1) plus++;
            else if (d == -1) minus++;
            else if (d != 0) return false;
        }
        return cycle ? plus == 0 : plus <= 1 && plus == minus;
    }
    int odd = 0;
    for (int v = 0; v < n; v++) odd += deg[v] % 2;
    return cycle ? odd == 0 : odd == 0 || odd == 2;
}

int main() {
    for (int it = 0; it < 2000; it++) {
        int n = (int)test::rnd(1, 6), m = (int)test::rnd(0, 8);
        vector<pair<int, int>> edges;
        for (int e = 0; e < m; e++) edges.push_back({(int)test::rnd(0, n - 1), (int)test::rnd(0, n - 1)});
        for (int directed = 0; directed < 2; directed++)
            for (int cycle = 0; cycle < 2; cycle++) {
                auto res = eulerianPath(n, edges, directed, cycle);
                CHECK_EQ(res.has_value(), exists(n, edges, directed, cycle));
                if (!res) continue;
                auto &[verts, ids] = *res;
                CHECK_EQ((int)ids.size(), m);
                CHECK_EQ((int)verts.size(), m + 1);
                CHECK_EQ((int)set<int>(ids.begin(), ids.end()).size(), m);
                for (int i = 0; i < m; i++) {
                    auto [u, v] = edges[ids[i]];
                    bool fwd = u == verts[i] && v == verts[i + 1];
                    bool bwd = !directed && v == verts[i] && u == verts[i + 1];
                    CHECK(fwd || bwd);
                }
                if (cycle) CHECK_EQ(verts.front(), verts.back());
            }
    }
}
