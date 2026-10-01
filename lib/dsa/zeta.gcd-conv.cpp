// Title: GCD convolution
// Description: c[k] = sum over gcd(i, j) == k of a[i] b[j] on indices 1..n, exact or modulo mod.
// Usage:
//   vector<long long> c = gcdConv(a, b);         // a, b on indices 1..n (index 0 ignored), exact
//   vector<long long> c = gcdConv(a, b, mod);    // modulo mod (inputs may be negative or unreduced)
//   c.size() = max(a.size(), b.size()), c[0] = 0
// Complexity: O(n log log n).
// Verify: https://judge.yosupo.jp/problem/gcd_convolution
inline vector<long long> gcdConv(vector<long long> a, vector<long long> b, long long mod = 0) {
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
    // A[d] = sum of a[m] over multiples m of d; then A[d] B[d] counts pairs with d | gcd
    // (one pass per prime, like a prefix sum along each prime's exponent)
    auto zeta = [&](vector<long long> &v) {
        v[0] = 0;
        for (auto &x : v) x = norm(x);
        for (int p : primes)
            for (int i = (n - 1) / p; i >= 1; i--) v[i] = norm(v[i] + v[i * p]);
    };
    zeta(a), zeta(b);
    vector<long long> c(n);
    for (int i = 0; i < n; i++) c[i] = mod ? a[i] * b[i] % mod : a[i] * b[i];
    for (int p : primes)  // inverse (Mobius) pass
        for (int i = 1; i <= (n - 1) / p; i++) c[i] = norm(c[i] - c[i * p]);
    return c;
}
