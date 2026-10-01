// Title: LCM convolution
// Description: c[k] = sum over lcm(i, j) == k of a[i] b[j] on indices 1..n, exact or modulo mod.
// Usage:
//   vector<long long> c = lcmConv(a, b);         // a, b on indices 1..n (index 0 ignored), exact
//   vector<long long> c = lcmConv(a, b, mod);    // modulo mod (inputs may be negative or unreduced)
//   c.size() = max(a.size(), b.size()), c[0] = 0; lcms above n are dropped
// Complexity: O(n log log n).
// Verify: https://judge.yosupo.jp/problem/lcm_convolution
inline vector<long long> lcmConv(vector<long long> a, vector<long long> b, long long mod = 0) {
    int n = (int)max(a.size(), b.size());
    a.resize(n), b.resize(n);
    if (!n) return a;
    auto norm = [&](long long x) { return mod ? (x % mod + mod) % mod : x; };
    vector<int> primes;
    vector<char> comp(n, 0);
    for (int i = 2; i < n; i++) {
        if (comp[i]) continue;
        primes.push_back(i);
        for (long long j = (long long)i * i; j < n; j += i) comp[j] = 1;
    }
    // A[m] = sum of a[d] over divisors d of m; then A[m] B[m] counts pairs with lcm | m
    // (one pass per prime, like a prefix sum along each prime's exponent)
    auto zeta = [&](vector<long long> &v) {
        v[0] = 0;
        for (auto &x : v) x = norm(x);
        for (int p : primes)
            for (int i = 1; i <= (n - 1) / p; i++) v[i * p] = norm(v[i * p] + v[i]);
    };
    zeta(a), zeta(b);
    vector<long long> c(n);
    for (int i = 0; i < n; i++) c[i] = mod ? a[i] * b[i] % mod : a[i] * b[i];
    for (int p : primes)  // inverse (Mobius) pass
        for (int i = (n - 1) / p; i >= 1; i--) c[i * p] = norm(c[i * p] - c[i]);
    return c;
}
