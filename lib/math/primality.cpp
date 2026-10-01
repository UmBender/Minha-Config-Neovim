// Title: Primality test
// Description: Deterministic Miller-Rabin for every 64-bit integer, plus 64-bit modular helpers.
// Usage:
//   isPrime(n)               // unsigned 64-bit n; 0 and 1 are not prime
//   mulMod64(a, b, m)        // a * b mod m without overflow (__int128), any m < 2^64
//   powMod64(b, e, m)        // b^e mod m
// Complexity: O(log n) multiplications (7 bases).
// Verify: https://judge.yosupo.jp/problem/primality_test
unsigned long long mulMod64(unsigned long long a, unsigned long long b, unsigned long long m) {
    return (unsigned long long)((unsigned __int128)a * b % m);
}

unsigned long long powMod64(unsigned long long b, unsigned long long e, unsigned long long m) {
    unsigned long long r = 1 % m;
    for (b %= m; e; e >>= 1, b = mulMod64(b, b, m))
        if (e & 1) r = mulMod64(r, b, m);
    return r;
}

bool isPrime(unsigned long long n) {
    if (n < 2) return false;
    for (unsigned long long p : {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37})
        if (n % p == 0) return n == p;
    int s = __builtin_ctzll(n - 1);
    unsigned long long d = (n - 1) >> s;
    // these 7 bases are enough for every n < 2^64
    for (unsigned long long a : {2, 325, 9375, 28178, 450775, 9780504, 1795265022}) {
        unsigned long long x = powMod64(a, d, n);
        if (x == 0 || x == 1 || x == n - 1) continue;  // x == 0: n divides a, the base says nothing
        for (int i = 1; i < s && x != n - 1; i++) x = mulMod64(x, x, n);
        if (x != n - 1) return false;
    }
    return true;
}
