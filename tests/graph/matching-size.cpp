#include "test.h"
#include "graph/matching-size.cpp"

int bruteMatching(int n, const vector<vector<char>> &adj) {
    vector<int> dp(1 << n, 0);
    for (int mask = 1; mask < (1 << n); mask++) {
        int v = __builtin_ctz(mask), rest = mask ^ (1 << v);
        dp[mask] = dp[rest];
        for (int u = 0; u < n; u++)
            if ((rest >> u & 1) && adj[v][u]) dp[mask] = max(dp[mask], 1 + dp[rest ^ (1 << u)]);
    }
    return dp[(1 << n) - 1];
}

int main() {
    CHECK_EQ(matchingSize(0, {}), 0);
    // two disjoint triangles: a symmetric (not skew) matrix would have full rank 6
    for (int rep = 0; rep < 20; rep++) CHECK_EQ(matchingSize(6, {{0, 1}, {1, 2}, {2, 0}, {3, 4}, {4, 5}, {5, 3}}), 2);
    for (int it = 0; it < 500; it++) {
        int n = (int)test::rnd(1, 12), m = (int)test::rnd(0, 30);
        vector<pair<int, int>> edges;
        vector<vector<char>> adj(n, vector<char>(n, 0));
        for (int e = 0; e < m; e++) {
            int u = (int)test::rnd(0, n - 1), v = (int)test::rnd(0, n - 1);
            edges.push_back({u, v});
            if (u != v) adj[u][v] = adj[v][u] = 1;
        }
        CHECK_EQ(matchingSize(n, edges), bruteMatching(n, adj));
    }
}
