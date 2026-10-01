// Title: Lagrange interpolation
// Description: Value at x of the polynomial of degree < n through n points modulo a prime; O(n) for nodes 0..n-1.
// Usage:
//   lagrange(xs, ys, x, mod)    // distinct nodes xs (mod p), of any sign
//   lagrange(ys, x, mod)        // nodes 0, 1, ..., n-1 (ys[i] = P(i)), n <= mod
//   // prime mod < 2^31 (998244353 by default), x any long long (e.g. 1e18)
// Complexity: O(n^2 + n log mod); nodes 0..n-1: O(n + log mod).
// Requires: math/powm
long long lagrange(const vector<long long> &xs, const vector<long long> &ys, long long x, long long mod = 998244353) {
    auto norm = [&](long long v) { return (v % mod + mod) % mod; };
    int n = (int)xs.size();
    long long ans = 0;
    x = norm(x);
    for (int i = 0; i < n; i++) {
        long long num = norm(ys[i]), den = 1;
        for (int j = 0; j < n; j++)
            if (j != i) num = num * norm(x - xs[j]) % mod, den = den * norm(xs[i] - xs[j]) % mod;
        ans = (ans + num * powMod(den, mod - 2, mod)) % mod;
    }
    return ans;
}

long long lagrange(const vector<long long> &ys, long long x, long long mod = 998244353) {
    int n = (int)ys.size();
    x = (x % mod + mod) % mod;
    if (n == 0) return 0;
    if (x < n) return (ys[x] % mod + mod) % mod;
    vector<long long> pre(n + 1, 1), suf(n + 1, 1), ifact(n, 1);  // products of (x - j) for j < i / j >= i
    for (int i = 0; i < n; i++) pre[i + 1] = pre[i] * (x - i) % mod;
    for (int i = n - 1; i >= 0; i--) suf[i] = suf[i + 1] * (x - i) % mod;
    long long f = 1;
    for (int i = 1; i < n; i++) f = f * i % mod;
    ifact[n - 1] = powMod(f, mod - 2, mod);
    for (int i = n - 1; i > 0; i--) ifact[i - 1] = ifact[i] * i % mod;
    long long ans = 0;
    for (int i = 0; i < n; i++) {  // denominator: i! (n-1-i)! (-1)^(n-1-i)
        long long t = pre[i] * suf[i + 1] % mod * ifact[i] % mod * ifact[n - 1 - i] % mod * ((ys[i] % mod + mod) % mod) % mod;
        ans = ((n - 1 - i) % 2 ? ans - t + mod : ans + t) % mod;
    }
    return ans;
}
