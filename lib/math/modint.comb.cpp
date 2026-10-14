// Title: Binomials modulo a prime
// Description: Inverses of 1..n in O(n), factorials and inverse factorials for C(n, k), P(n, k) with ModInt.
// Usage:
//   Comb<Mint> cb(n);          // tables up to n (optional: they grow on demand); prime mod > n
//   cb.C(n, k); cb.P(n, k);    // binomial, arrangements; 0 if n < 0, k < 0 or k > n
//   cb.fact(i); cb.ifact(i); cb.inv(i);   // i!, 1 / i!, 1 / i (i >= 1)
//   // dynamic modulus: build the Comb after setMod (the tables belong to one modulus)
// Complexity: O(n) to build up to n, O(1) per query.
// Requires: math/modint
template <class Mint> struct Comb {
    vector<Mint> f{1}, fi{1}, iv{0};

    Comb(int n = 0) { grow(n); }
    void grow(int n) {
        for (int i = (int)f.size(); i <= n; i++) {
            int p = Mint::mod();
            iv.push_back(i == 1 ? Mint(1) : -Mint(p / i) * iv[p % i]);  // p = (p / i) i + p % i
            f.push_back(f[i - 1] * i);
            fi.push_back(fi[i - 1] * iv[i]);
        }
    }
    Mint fact(int i) { return grow(i), f[i]; }
    Mint ifact(int i) { return grow(i), fi[i]; }
    Mint inv(int i) { return grow(i), iv[i]; }
    Mint C(int n, int k) {
        if (n < 0 || k < 0 || k > n) return 0;
        return grow(n), f[n] * fi[k] * fi[n - k];
    }
    Mint P(int n, int k) {
        if (n < 0 || k < 0 || k > n) return 0;
        return grow(n), f[n] * fi[n - k];
    }
};
