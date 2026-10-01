#include "test.h"
#include "graph/matching.cpp"

int bruteMatching(int n, const vector<vector<char>> &adj) {
    vector<int> dp(1 << n, 0);  // dp[mask] = max matching within the vertices of mask
    for (int mask = 1; mask < (1 << n); mask++) {
        int v = __builtin_ctz(mask), rest = mask ^ (1 << v);
        dp[mask] = dp[rest];
        for (int u = 0; u < n; u++)
            if ((rest >> u & 1) && adj[v][u]) dp[mask] = max(dp[mask], 1 + dp[rest ^ (1 << u)]);
    }
    return dp[(1 << n) - 1];
}

int main() {
    CHECK(generalMatching({}).empty());
    for (int it = 0; it < 600; it++) {
        int n = (int)test::rnd(1, 12), m = (int)test::rnd(0, 30);
        vector<vector<int>> g(n);
        vector<vector<char>> adj(n, vector<char>(n, 0));
        for (int e = 0; e < m; e++) {
            int u = (int)test::rnd(0, n - 1), v = (int)test::rnd(0, n - 1);
            g[u].push_back(v), g[v].push_back(u);  // self-loops and multi-edges included
            if (u != v) adj[u][v] = adj[v][u] = 1;
        }
        auto mate = generalMatching(g);
        CHECK_EQ((int)mate.size(), n);
        int cnt = 0;
        for (int v = 0; v < n; v++)
            if (mate[v] != -1) {
                CHECK(adj[v][mate[v]]);
                CHECK_EQ(mate[mate[v]], v);
                cnt++;
            }
        CHECK_EQ(cnt / 2, bruteMatching(n, adj));
    }
}
