// Title: Modular power
// Description: b^e mod m by binary exponentiation; inverse modulo a prime as powMod(a, p - 2, p).
// Usage:
//   powMod(2, 10)                 // 1024 (default modulus 998244353)
//   powMod(b, e, 1000000007)      // any modulus 1 <= mod < 2^31, e >= 0, b of any sign/size
//   powMod(a, p - 2, p)           // inverse of a modulo a prime p (p does not divide a)
// Complexity: O(log e).
long long powMod(long long b, long long e, long long mod = 998244353) {
    long long r = 1 % mod;
    for (b = (b % mod + mod) % mod; e; e >>= 1, b = b * b % mod)
        if (e & 1) r = r * b % mod;
    return r;
}
