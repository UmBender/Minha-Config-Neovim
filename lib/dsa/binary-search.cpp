// Title: Binary search
// Description: First/last integer where a monotone predicate holds (any integer type, overflow-safe); real version.
// Usage:
//   firstTrue(lo, hi, pred)  smallest x in [lo, hi) with pred(x); hi if none. pred: false..false true..true
//   lastTrue(lo, hi, pred)   largest x in [lo, hi) with pred(x); lo - 1 if none. pred: true..true false..false
//   firstTrueReal(lo, hi, pred, iters = 100)  boundary in [lo, hi] of a monotone predicate on reals
//   int i = firstTrue(0, n, [&](int i) { return a[i] >= x; });           // lower_bound
//   long long k = firstTrue(0LL, (long long)2e18, [&](long long k) { return f(k) >= target; });
// Complexity: O(log(hi - lo)) predicate calls.
// Pending: example + presets (T-009..T-011), remove when done
template <class T, class F> T firstTrue(T lo, T hi, F pred) {
    while (lo < hi) {
        T mid = midpoint(lo, hi);  // rounds down, never overflows
        if (pred(mid)) hi = mid;
        else lo = mid + 1;
    }
    return lo;
}

template <class T, class F> T lastTrue(T lo, T hi, F pred) {
    return firstTrue(lo, hi, [&](T x) { return !pred(x); }) - 1;
}

template <class T, class F> T firstTrueReal(T lo, T hi, F pred, int iters = 100) {
    for (int it = 0; it < iters; it++) {
        T mid = (lo + hi) / 2;
        if (pred(mid)) hi = mid;
        else lo = mid;
    }
    return hi;
}
