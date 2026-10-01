// Title: Modular square root
// Description: Smallest x with x^2 = a (mod p) for a prime p (Tonelli-Shanks), or -1 if a is not a square.
// Usage:
//   sqrtMod(a, p)    // prime p < 2^31 (998244353 by default), any a (reduced first); the other root is p - x
// Complexity: O(log^2 p) multiplications.
// Verify: https://judge.yosupo.jp/problem/sqrt_mod
// Requires: math/powm
long long sqrtMod(long long a, long long p = 998244353) {
    a = (a % p + p) % p;
    if (a == 0 || p == 2) return a;
    if (powMod(a, (p - 1) / 2, p) != 1) return -1;  // Euler's criterion
    long long x;
    if (p % 4 == 3) {
        x = powMod(a, (p + 1) / 4, p);
    } else {
        int s = __builtin_ctzll(p - 1);
        long long q = (p - 1) >> s, z = 2;  // p - 1 = q * 2^s, z a non-residue
        while (powMod(z, (p - 1) / 2, p) != p - 1) z++;
        long long c = powMod(z, q, p), t = powMod(a, q, p);
        x = powMod(a, (q + 1) / 2, p);  // invariant: x^2 = a * t
        for (int m = s; t != 1;) {
            int i = 0;  // order of t is 2^i
            for (long long tt = t; tt != 1; tt = tt * tt % p) i++;
            long long b = powMod(c, 1LL << (m - i - 1), p);
            x = x * b % p, c = b * b % p, t = t * c % p, m = i;
        }
    }
    return min(x, p - x);
}
