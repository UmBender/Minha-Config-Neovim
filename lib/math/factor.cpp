// Title: Integer factorization
// Description: Prime factors of a 64-bit integer (Pollard rho + Miller-Rabin), sorted, with multiplicity.
// Usage:
//   vector<unsigned long long> fs = factor(n);   // n >= 1: factor(12) = {2, 2, 3}, factor(1) = {}
//   pollardRho(n)                                // some nontrivial divisor of a composite n
// Complexity: about O(n^(1/4)) multiplications (expected).
// Verify: https://judge.yosupo.jp/problem/factorize
// Requires: math/primality
unsigned long long pollardRho(unsigned long long n) {
    if (n % 2 == 0) return 2;
    for (unsigned long long c = 1;; c++) {
        auto f = [&](unsigned long long v) { return (mulMod64(v, v, n) + c) % n; };
        unsigned long long x = 0, y = 0, prod = 1;
        for (int i = 1;; i++) {
            x = f(x), y = f(f(y));
            unsigned long long diff = x > y ? x - y : y - x, q = mulMod64(prod, diff, n);
            if (q == 0) {  // this step (or the batch) hit every factor at once: check both, else next c
                for (unsigned long long g : {gcd(diff, n), gcd(prod, n)})
                    if (g != 1 && g != n) return g;
                break;
            }
            prod = q;  // gcd once per 64 steps
            if (i % 64 == 0 && gcd(prod, n) != 1) return gcd(prod, n);
        }
    }
}

vector<unsigned long long> factor(unsigned long long n) {
    if (n == 1) return {};
    if (isPrime(n)) return {n};
    unsigned long long d = pollardRho(n);
    auto fs = factor(d), rest = factor(n / d);
    fs.insert(fs.end(), rest.begin(), rest.end());
    sort(fs.begin(), fs.end());
    return fs;
}
