// Title: Floor sums
// Description: Sums of floor((a*i + b) / m), i * floor(...) and floor(...)^2 over 0 <= i < n, any sign of a, b.
// Usage:
//   floorSum(n, m, a, b)                       // sum_{0 <= i < n} floor((a*i + b) / m)        (__int128)
//   floorSumI(n, m, a, b)                      // sum i * floor((a*i + b) / m)
//   floorSumSq(n, m, a, b)                     // sum floor((a*i + b) / m)^2
//   auto [f, g, h] = floorSums(n, m, a, b);    // the three at once
//   // n >= 0, m >= 1; results (and terms of their size) must fit in __int128
// Complexity: O(log m).
// Verify: https://judge.yosupo.jp/problem/sum_of_floor_of_linear
__int128 floorSum(long long n, long long m, long long a, long long b) {
    if (n == 0) return 0;
    long long qa = a / m - (a % m < 0), qb = b / m - (b % m < 0);  // floor division
    a -= qa * m, b -= qb * m;
    __int128 res = (__int128)qa * n * (n - 1) / 2 + (__int128)qb * n;
    long long k = (long long)(((__int128)a * (n - 1) + b) / m);  // largest value, now 0 <= a, b < m
    return k ? res + (__int128)k * (n - 1) - floorSum(k, a, m, m - b - 1) : res;
}

// One recursion for the three sums: calling floorSumSq / floorSumI recursively would grow like Fibonacci.
array<__int128, 3> floorSums(long long n, long long m, long long a, long long b) {
    if (n == 0) return {0, 0, 0};
    long long qa = a / m - (a % m < 0), qb = b / m - (b % m < 0);
    a -= qa * m, b -= qb * m;
    __int128 s1 = (__int128)n * (n - 1) / 2, s2 = s1 * (2 * n - 1) / 3;  // sum i, sum i^2
    __int128 f = 0, g = 0, h = 0;
    long long k = (long long)(((__int128)a * (n - 1) + b) / m);
    if (k) {  // swap the roles of i and the floor value
        auto [f2, g2, h2] = floorSums(k, a, m, m - b - 1);
        f = (__int128)k * (n - 1) - f2;
        g = k * s1 - (h2 + f2) / 2;
        h = (__int128)k * k * (n - 1) - 2 * g2 - f2;
    }
    // floor((a*i + b) / m) = qa*i + qb + (the reduced floor)
    return {qa * s1 + qb * n + f, qa * s2 + qb * s1 + g,
            qa * qa * s2 + 2 * qa * qb * s1 + (__int128)qb * qb * n + 2 * qa * g + 2 * qb * f + h};
}

__int128 floorSumI(long long n, long long m, long long a, long long b) { return floorSums(n, m, a, b)[1]; }

__int128 floorSumSq(long long n, long long m, long long a, long long b) { return floorSums(n, m, a, b)[2]; }
