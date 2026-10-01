// Title: Berlekamp-Massey
// Description: Shortest linear recurrence s[i] = sum c[j] s[i-1-j] generating a sequence, modulo a prime.
// Usage:
//   vector<long long> c = berlekampMassey(s, mod);   // s[i] = c[0] s[i-1] + ... + c[L-1] s[i-L] for i >= L
//   linRec(s, c, k, mod)                             // then the k-th term (math/linear-recurrence)
//   // 2L terms are enough to recover a recurrence of length L; prime mod < 2^31 (998244353 by default)
// Complexity: O(n^2).
// Verify: https://judge.yosupo.jp/problem/find_linear_recurrence
// Requires: math/powm
vector<long long> berlekampMassey(vector<long long> s, long long mod = 998244353) {
    int n = (int)s.size(), l = 0, m = 0;
    for (auto &x : s) x = (x % mod + mod) % mod;
    vector<long long> c(n + 1, 0), b(n + 1, 0), t;  // s[i] + sum c[j] s[i-j] = 0; b: c before the last length change
    c[0] = b[0] = 1;
    long long bd = 1;  // discrepancy when b was saved
    for (int i = 0; i < n; i++) {
        m++;
        long long d = s[i];
        for (int j = 1; j <= l; j++) d = (d + c[j] * s[i - j]) % mod;
        if (d == 0) continue;
        t = c;
        long long coef = d * powMod(bd, mod - 2, mod) % mod;
        for (int j = m; j <= n; j++) c[j] = (c[j] - coef * b[j - m] % mod + mod) % mod;
        if (2 * l > i) continue;
        l = i + 1 - l, b = t, bd = d, m = 0;
    }
    c.resize(l + 1);
    c.erase(c.begin());
    for (auto &x : c) x = (mod - x) % mod;
    return c;
}
