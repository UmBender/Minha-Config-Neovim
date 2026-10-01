// Title: Zeta and Mobius transforms
// Description: Sums over subsets/supersets (bitmask) and over divisors/multiples, plus their inverses.
// Usage:
//   All take the vector by value and return the transformed one (any numeric T).
//   subsetZeta(a)       b[S] = sum of a[T], T subset of S      (a.size() a power of two)
//   subsetZeta(a, op)   any associative + commutative op, e.g. max:  [](T x, T y) { return max(x, y); }
//   supersetZeta(a[, op])   b[S] = sum of a[T], T superset of S
//   subsetMobius / supersetMobius     inverses (for sums)
//   divisorZeta(a)      b[m] = sum of a[d], d | m        (indices 1..n, a.size() = n + 1, a[0] untouched)
//   multipleZeta(a)     b[d] = sum of a[m], d | m <= n
//   divisorMobius / multipleMobius    inverses
//   gcd convolution: multipleMobius(multipleZeta(a) * multipleZeta(b) pointwise)
// Complexity: O(n log n) for masks (n = 2^k), O(n log log n) for divisors.
// Verify: https://judge.yosupo.jp/problem/gcd_convolution
// Pending: example + presets (T-009..T-011), remove when done
template <class T, class Op = plus<T>> vector<T> subsetZeta(vector<T> a, Op op = Op()) {
    int n = (int)a.size();
    for (int j = 1; j < n; j <<= 1)
        for (int i = 0; i < n; i++)
            if (i & j) a[i] = op(a[i], a[i ^ j]);
    return a;
}

template <class T, class Op = plus<T>> vector<T> supersetZeta(vector<T> a, Op op = Op()) {
    int n = (int)a.size();
    for (int j = 1; j < n; j <<= 1)
        for (int i = 0; i < n; i++)
            if (i & j) a[i ^ j] = op(a[i ^ j], a[i]);
    return a;
}

template <class T> vector<T> subsetMobius(vector<T> a) {
    int n = (int)a.size();
    for (int j = 1; j < n; j <<= 1)
        for (int i = 0; i < n; i++)
            if (i & j) a[i] -= a[i ^ j];
    return a;
}

template <class T> vector<T> supersetMobius(vector<T> a) {
    int n = (int)a.size();
    for (int j = 1; j < n; j <<= 1)
        for (int i = 0; i < n; i++)
            if (i & j) a[i ^ j] -= a[i];
    return a;
}

inline vector<int> zetaPrimes(int n) {  // primes <= n
    vector<int> pr;
    vector<char> comp(n + 1, 0);
    for (int i = 2; i <= n; i++) {
        if (comp[i]) continue;
        pr.push_back(i);
        for (long long j = (long long)i * i; j <= n; j += i) comp[j] = 1;
    }
    return pr;
}

template <class T> vector<T> divisorZeta(vector<T> a) {
    int n = (int)a.size() - 1;
    for (int p : zetaPrimes(n))
        for (int i = 1; i <= n / p; i++) a[i * p] += a[i];
    return a;
}

template <class T> vector<T> divisorMobius(vector<T> a) {
    int n = (int)a.size() - 1;
    for (int p : zetaPrimes(n))
        for (int i = n / p; i >= 1; i--) a[i * p] -= a[i];
    return a;
}

template <class T> vector<T> multipleZeta(vector<T> a) {
    int n = (int)a.size() - 1;
    for (int p : zetaPrimes(n))
        for (int i = n / p; i >= 1; i--) a[i] += a[i * p];
    return a;
}

template <class T> vector<T> multipleMobius(vector<T> a) {
    int n = (int)a.size() - 1;
    for (int p : zetaPrimes(n))
        for (int i = 1; i <= n / p; i++) a[i] -= a[i * p];
    return a;
}
