// Title: Lagrange polynomial
// Description: Coefficients of the polynomial of degree < n through n points, modulo a prime.
// Usage:
//   vector<long long> c = lagrangePoly(xs, ys, mod);   // P(x) = c[0] + c[1] x + ... + c[n-1] x^(n-1)
//   // distinct nodes xs (mod p) of any sign; prime mod < 2^31 (998244353 by default)
// Complexity: O(n^2 + n log mod).
// Requires: math/powm
vector<long long> lagrangePoly(const vector<long long> &xs, const vector<long long> &ys, long long mod = 998244353) {
    auto norm = [&](long long v) { return (v % mod + mod) % mod; };
    int n = (int)xs.size();
    vector<long long> p = {1}, q(n), f(n, 0);  // p = prod (x - xs[i])
    for (int i = 0; i < n; i++) {
        long long xi = norm(xs[i]);
        p.push_back(0);
        for (int j = i + 1; j > 0; j--) p[j] = (p[j - 1] + mod - p[j] * xi % mod) % mod;
        p[0] = p[0] * (mod - xi) % mod;
    }
    for (int i = 0; i < n; i++) {
        long long xi = norm(xs[i]), den = 1;
        q[n - 1] = p[n];  // q = p / (x - xs[i])
        for (int j = n - 1; j > 0; j--) q[j - 1] = (p[j] + q[j] * xi) % mod;
        for (int j = 0; j < n; j++)
            if (j != i) den = den * norm(xs[i] - xs[j]) % mod;
        long long c = norm(ys[i]) * powMod(den, mod - 2, mod) % mod;
        for (int j = 0; j < n; j++) f[j] = (f[j] + c * q[j]) % mod;
    }
    return f;
}
