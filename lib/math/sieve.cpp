// Title: Linear sieve
// Description: Primes below n and the smallest prime factor of every x < n; factorization in O(log x).
// Usage:
//   Sieve sv(n);        // for [0, n)
//   sv.primes           // 2, 3, 5, ... < n
//   sv.spf[x]           // smallest prime factor of x (0 for x < 2)
//   sv.isPrime(x)       // 0 <= x < n
//   sv.factor(x)        // prime factors of 1 <= x < n, sorted, with multiplicity
// Complexity: O(n) build, O(log x) per factor.
// Verify: https://judge.yosupo.jp/problem/enumerate_primes
struct Sieve {
    vector<int> primes, spf;
    Sieve(int n) : spf(max(n, 0)) {
        for (int i = 2; i < n; i++) {
            if (!spf[i]) spf[i] = i, primes.push_back(i);
            for (int p : primes) {  // i * p gets its smallest factor p exactly once
                if (p > spf[i] || (long long)i * p >= n) break;
                spf[i * p] = p;
            }
        }
    }
    bool isPrime(int x) const { return x >= 2 && spf[x] == x; }
    vector<int> factor(int x) const {
        vector<int> fs;
        for (; x > 1; x /= spf[x]) fs.push_back(spf[x]);
        return fs;
    }
};
