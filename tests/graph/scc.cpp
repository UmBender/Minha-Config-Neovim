#include "test.h"
#include "graph/scc.cpp"

int main() {
    for (int it = 0; it < 400; it++) {
        int n = (int)test::rnd(1, 9), m = (int)test::rnd(0, 18);
        vector<vector<int>> g(n);
        vector<vector<char>> reach(n, vector<char>(n, 0));
        for (int i = 0; i < n; i++) reach[i][i] = 1;
        for (int e = 0; e < m; e++) {
            int u = (int)test::rnd(0, n - 1), v = (int)test::rnd(0, n - 1);
            g[u].push_back(v), reach[u][v] = 1;
        }
        for (int k = 0; k < n; k++)
            for (int i = 0; i < n; i++)
                for (int j = 0; j < n; j++)
                    if (reach[i][k] && reach[k][j]) reach[i][j] = 1;
        SCC s(g);
        for (int u = 0; u < n; u++)
            for (int v = 0; v < n; v++) CHECK_EQ(s.comp[u] == s.comp[v], reach[u][v] && reach[v][u]);
        set<int> ids(s.comp.begin(), s.comp.end());
        CHECK_EQ((int)ids.size(), s.count);
        CHECK(*ids.rbegin() == s.count - 1);
        // topological: edges go from smaller to larger component ids
        for (int u = 0; u < n; u++)
            for (int v : g[u]) CHECK(s.comp[u] <= s.comp[v]);
        auto comps = s.components();
        CHECK_EQ((int)comps.size(), s.count);
        for (int c = 0; c < s.count; c++)
            for (int v : comps[c]) CHECK_EQ(s.comp[v], c);
        auto dag = s.condensation(g);
        set<pair<int, int>> want;
        for (int u = 0; u < n; u++)
            for (int v : g[u])
                if (s.comp[u] != s.comp[v]) want.insert({s.comp[u], s.comp[v]});
        set<pair<int, int>> got;
        for (int c = 0; c < s.count; c++) {
            CHECK(is_sorted(dag[c].begin(), dag[c].end()));
            for (int d : dag[c]) CHECK(got.insert({c, d}).second);
        }
        CHECK_EQ(got, want);
    }
    // long path: no recursion depth issues
    int n = 200000;
    vector<vector<int>> g(n);
    for (int i = 0; i + 1 < n; i++) g[i].push_back(i + 1);
    g[n - 1].push_back(0);
    CHECK_EQ(SCC(g).count, 1);
}
