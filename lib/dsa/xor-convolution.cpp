// Title: XOR, AND, OR convolution
// Description: Bitwise convolutions with the fast Walsh-Hadamard / zeta transforms, exact or modulo a prime.
// Usage:
//   xorConv(a, b)       c[k] = sum over i ^ j == k of a[i] b[j]   (exact integers)
//   andConv(a, b)       ... i & j == k;      orConv(a, b)   ... i | j == k
//   xorConv(a, b, mod)  same modulo a prime mod (mod = 0 means no modulus)
//   The result has size = next power of two >= max(a.size(), b.size()).
// Complexity: O(n log n).
// Verify: https://judge.yosupo.jp/problem/bitwise_xor_convolution
// Verify: https://judge.yosupo.jp/problem/bitwise_and_convolution
inline void bitTransform(vector<long long> &a, int kind, bool inverse, long long mod) {
    int n = (int)a.size();
    for (int k = 1; k < n; k <<= 1)
        for (int i = 0; i < n; i += 2 * k)
            for (int j = i; j < i + k; j++) {
                long long &x = a[j], &y = a[j + k];
                if (kind == 0) {
                    long long u = x, v = y;
                    x = u + v, y = u - v;
                } else if (kind == 1) {
                    x += inverse ? -y : y;  // and: sum over supersets
                } else {
                    y += inverse ? -x : x;  // or: sum over subsets
                }
                if (mod) x = (x % mod + mod) % mod, y = (y % mod + mod) % mod;
            }
    if (kind == 0 && inverse) {
        if (!mod) {
            for (auto &x : a) x /= n;
            return;
        }
        long long inv = 1, base = n % mod;
        for (long long e = mod - 2; e; e >>= 1, base = base * base % mod)
            if (e & 1) inv = inv * base % mod;
        for (auto &x : a) x = x * inv % mod;
    }
}

inline vector<long long> bitConv(vector<long long> a, vector<long long> b, long long mod, int kind) {
    int n = 1;
    while (n < (int)max(a.size(), b.size())) n <<= 1;
    a.resize(n), b.resize(n);
    if (mod)
        for (auto *v : {&a, &b})
            for (auto &x : *v) x = (x % mod + mod) % mod;
    bitTransform(a, kind, false, mod), bitTransform(b, kind, false, mod);
    for (int i = 0; i < n; i++) a[i] = mod ? a[i] * b[i] % mod : a[i] * b[i];
    bitTransform(a, kind, true, mod);
    return a;
}

inline vector<long long> xorConv(const vector<long long> &a, const vector<long long> &b, long long mod = 0) {
    return bitConv(a, b, mod, 0);
}
inline vector<long long> andConv(const vector<long long> &a, const vector<long long> &b, long long mod = 0) {
    return bitConv(a, b, mod, 1);
}
inline vector<long long> orConv(const vector<long long> &a, const vector<long long> &b, long long mod = 0) {
    return bitConv(a, b, mod, 2);
}
