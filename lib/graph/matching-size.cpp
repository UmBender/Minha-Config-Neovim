// Title: Matching size (Tutte matrix)
// Description: Size of a maximum matching in a general graph, randomized rank of the Tutte matrix.
// Usage:
//   int k = matchingSize(n, edges);   // edges: vector<pair<int, int>>, undirected; loops ignored
//   wrong with probability <= n / 1e9 (it can only underestimate)
// Complexity: O(n^3).
int matchingSize(int n, const vector<pair<int, int>> &edges) {
    const long long MOD = 1'000'000'007;
    static mt19937_64 rng((unsigned long long)chrono::steady_clock::now().time_since_epoch().count());
    auto power = [&](long long b, long long e) {
        long long r = 1;
        for (b %= MOD; e; e >>= 1, b = b * b % MOD)
            if (e & 1) r = r * b % MOD;
        return r;
    };
    vector<vector<long long>> a(n, vector<long long>(n, 0));
    for (auto [u, v] : edges) {
        if (u == v) continue;
        long long x = (long long)(rng() % (MOD - 1)) + 1;
        a[u][v] = (a[u][v] + x) % MOD, a[v][u] = (a[v][u] + MOD - x) % MOD;
    }
    int rank = 0;
    for (int col = 0; col < n && rank < n; col++) {
        int piv = rank;
        while (piv < n && a[piv][col] == 0) piv++;
        if (piv == n) continue;
        swap(a[piv], a[rank]);
        long long inv = power(a[rank][col], MOD - 2);
        for (int i = rank + 1; i < n; i++) {
            if (a[i][col] == 0) continue;
            long long f = a[i][col] * inv % MOD;
            for (int j = col; j < n; j++) a[i][j] = (a[i][j] - f * a[rank][j] % MOD + MOD) % MOD;
        }
        rank++;
    }
    return rank / 2;
}
