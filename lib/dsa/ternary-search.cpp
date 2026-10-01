// Title: Ternary search
// Description: Argmax/argmin of a unimodal function over integers (binary search on the slope) or reals.
// Usage:
//   ternaryMax(lo, hi, f)  x in [lo, hi] maximizing f; f strictly increasing up to the max, then
//                          non-increasing (plateaus only after the max). Returns the first maximum.
//   ternaryMin(lo, hi, f)  same for the minimum (strictly decreasing, then non-decreasing)
//   ternaryMaxReal(lo, hi, f, iters = 200) / ternaryMinReal(...)  for real arguments
//   long long best = ternaryMax(0LL, n - 1LL, [&](long long k) { return profit(k); });
// Complexity: O(log(hi - lo)) evaluations (integers), O(iters) (reals).
template <class T, class F> T ternaryMax(T lo, T hi, F f) {
    while (lo < hi) {
        T mid = midpoint(lo, hi);
        if (f(mid) < f(mid + 1)) lo = mid + 1;
        else hi = mid;
    }
    return lo;
}

template <class T, class F> T ternaryMin(T lo, T hi, F f) {
    while (lo < hi) {
        T mid = midpoint(lo, hi);
        if (f(mid + 1) < f(mid)) lo = mid + 1;
        else hi = mid;
    }
    return lo;
}

template <class T, class F> T ternaryMaxReal(T lo, T hi, F f, int iters = 200) {
    for (int it = 0; it < iters; it++) {
        T m1 = lo + (hi - lo) / 3, m2 = hi - (hi - lo) / 3;
        if (f(m1) < f(m2)) lo = m1;
        else hi = m2;
    }
    return (lo + hi) / 2;
}

template <class T, class F> T ternaryMinReal(T lo, T hi, F f, int iters = 200) {
    return ternaryMaxReal(lo, hi, [&](T x) { return -f(x); }, iters);
}
