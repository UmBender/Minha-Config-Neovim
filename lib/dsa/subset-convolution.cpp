// Title: Subset convolution
// Description: c[S] = sum over T subset of S of a[T] * b[S \ T] (disjoint union), exact or modulo.
// Usage:
//   vector<long long> c = subsetConvolution(a, b);          // a.size() == b.size() == 2^k
//   subsetConvolution(a, b, 998244353)                      // modulo (mod = 0 means exact)
// Complexity: O(2^k k^2).
// Verify: https://judge.yosupo.jp/problem/subset_convolution
// Pending: example + presets (T-009..T-011), remove when done
inline vector<long long> subsetConvolution(const vector<long long> &a, const vector<long long> &b, long long mod = 0) {
    int n = (int)a.size(), k = 0;
    while ((1 << k) < n) k++;
    auto norm = [&](long long x) { return mod ? (x % mod + mod) % mod : x; };
    vector<vector<long long>> fa(k + 1, vector<long long>(n)), fb = fa, h = fa;
    for (int i = 0; i < n; i++) {
        int pc = __builtin_popcount(i);
        fa[pc][i] = norm(a[i]), fb[pc][i] = norm(b[i]);
    }
    for (auto *f : {&fa, &fb})
        for (auto &v : *f)
            for (int j = 1; j < n; j <<= 1)
                for (int i = 0; i < n; i++)
                    if (i & j) v[i] = norm(v[i] + v[i ^ j]);
    for (int i = 0; i < n; i++)
        for (int x = 0; x <= k; x++)
            for (int y = 0; x + y <= k; y++) h[x + y][i] = norm(h[x + y][i] + norm(fa[x][i] * fb[y][i]));
    for (auto &v : h)
        for (int j = 1; j < n; j <<= 1)
            for (int i = 0; i < n; i++)
                if (i & j) v[i] = norm(v[i] - v[i ^ j]);
    vector<long long> res(n);
    for (int i = 0; i < n; i++) res[i] = h[__builtin_popcount(i)][i];
    return res;
}
