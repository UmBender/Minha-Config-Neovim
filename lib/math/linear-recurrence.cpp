// Title: Linear recurrence
// Description: k-th term of s[i] = sum c[j] s[i-1-j] modulo m for huge k (polynomial exponentiation).
// Usage:
//   linRec(s, c, k, mod)         // s: the first c.size() terms (at least), 0-indexed k up to 1e18
//   linRec({0, 1}, {1, 1}, k)    // Fibonacci F(k) mod 998244353
//   // any 1 <= mod < 2^31 (998244353 by default); empty c: every term from s.size() on is 0
// Complexity: O(L^2 log k), L = c.size().
long long linRec(vector<long long> s, vector<long long> c, long long k, long long mod = 998244353) {
    int n = (int)c.size();
    for (auto *v : {&s, &c})
        for (auto &x : *v) x = (x % mod + mod) % mod;
    if (k < (long long)s.size()) return s[k];
    if (n == 0) return 0;
    auto mul = [&](const vector<long long> &a, const vector<long long> &b) {  // a * b mod (x^n - sum c[j] x^(n-1-j))
        vector<long long> r(2 * n + 1, 0);
        for (int i = 0; i <= n; i++)
            for (int j = 0; j <= n; j++) r[i + j] = (r[i + j] + a[i] * b[j]) % mod;
        for (int i = 2 * n; i > n; i--)
            for (int j = 0; j < n; j++) r[i - 1 - j] = (r[i - 1 - j] + r[i] * c[j]) % mod;
        r.resize(n + 1);
        return r;
    };
    vector<long long> pol(n + 1, 0), e(n + 1, 0);
    pol[0] = e[1] = 1;  // x^(k+1), then read off s[k] = sum pol[i+1] s[i]
    for (++k; k; k >>= 1) {
        if (k & 1) pol = mul(pol, e);
        e = mul(e, e);
    }
    long long ans = 0;
    for (int i = 0; i < n; i++) ans = (ans + pol[i + 1] * s[i]) % mod;
    return ans;
}
