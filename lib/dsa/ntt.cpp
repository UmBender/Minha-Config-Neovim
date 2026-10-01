// Title: NTT convolution
// Description: Convolution modulo an NTT prime (998244353 by default) and modulo any number (3 primes + CRT).
// Usage:
//   vector<long long> c = nttConv(a, b);                  // mod 998244353 (values are reduced first)
//   nttConv<469762049, 3>(a, b)                           // other NTT prime <MOD, primitive root>
//   convMod(a, b, 1000000007)                             // any modulus m < 2^31
//   ntt<MOD, G>(a, invert)                                // in-place transform, a.size() a power of two
// Complexity: O(n log n) (convMod: 3 NTT convolutions).
// Verify: https://judge.yosupo.jp/problem/convolution_mod
// Verify: https://judge.yosupo.jp/problem/convolution_mod_1000000007
template <unsigned MOD> long long nttPow(long long b, long long e) {
    long long r = 1;
    for (b %= MOD; e; e >>= 1, b = b * b % MOD)
        if (e & 1) r = r * b % MOD;
    return r;
}

template <unsigned MOD = 998244353, unsigned G = 3> void ntt(vector<long long> &a, bool invert) {
    int n = (int)a.size();
    for (int i = 1, j = 0; i < n; i++) {
        int bit = n >> 1;
        for (; j & bit; bit >>= 1) j ^= bit;
        j ^= bit;
        if (i < j) swap(a[i], a[j]);
    }
    vector<long long> w(n / 2 + 1);
    for (int len = 2; len <= n; len <<= 1) {
        long long step = nttPow<MOD>(G, (MOD - 1) / len);
        if (invert) step = nttPow<MOD>(step, MOD - 2);
        w[0] = 1;
        for (int k = 1; k < len / 2; k++) w[k] = w[k - 1] * step % MOD;
        for (int i = 0; i < n; i += len)
            for (int k = 0; k < len / 2; k++) {
                long long u = a[i + k], v = a[i + k + len / 2] * w[k] % MOD;
                a[i + k] = u + v < MOD ? u + v : u + v - MOD;
                a[i + k + len / 2] = u - v >= 0 ? u - v : u - v + MOD;
            }
    }
    if (invert) {
        long long inv = nttPow<MOD>(n, MOD - 2);
        for (auto &x : a) x = x * inv % MOD;
    }
}

template <unsigned MOD = 998244353, unsigned G = 3> vector<long long> nttConv(vector<long long> a, vector<long long> b) {
    if (a.empty() || b.empty()) return {};
    int s = (int)(a.size() + b.size() - 1), n = 1;
    while (n < s) n <<= 1;
    for (auto *v : {&a, &b}) {
        for (auto &x : *v) x = (x % MOD + MOD) % MOD;
        v->resize(n);
        ntt<MOD, G>(*v, false);
    }
    for (int i = 0; i < n; i++) a[i] = a[i] * b[i] % MOD;
    ntt<MOD, G>(a, true);
    a.resize(s);
    return a;
}

inline vector<long long> convMod(vector<long long> a, vector<long long> b, long long m) {
    if (a.empty() || b.empty()) return {};
    for (auto *v : {&a, &b})
        for (auto &x : *v) x = (x % m + m) % m;
    constexpr unsigned M1 = 754974721, M2 = 167772161, M3 = 469762049;
    auto c1 = nttConv<M1, 11>(a, b), c2 = nttConv<M2, 3>(a, b), c3 = nttConv<M3, 3>(a, b);
    const long long inv12 = nttPow<M2>(M1 % M2, M2 - 2);
    const long long inv123 = nttPow<M3>((long long)(M1 % M3) * (M2 % M3) % M3, M3 - 2);
    const long long m1 = M1 % m, m12 = m1 * (M2 % m) % m;
    vector<long long> res(c1.size());
    for (size_t i = 0; i < res.size(); i++) {
        long long x1 = c1[i];
        long long x2 = (c2[i] - x1 % M2 + M2) % M2 * inv12 % M2;
        long long t = ((c3[i] - x1 % M3 - x2 * (M1 % M3) % M3) % M3 + 2LL * M3) % M3;
        long long x3 = t * inv123 % M3;
        res[i] = (x1 % m + x2 % m * m1 + x3 % m * m12) % m;
    }
    return res;
}
