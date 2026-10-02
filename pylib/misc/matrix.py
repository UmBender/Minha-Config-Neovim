# Title: Matrix power and recurrences
# Description: Matrix product and power (exact or mod m), Fibonacci by fast doubling, n-th term of a linear recurrence.
# Usage:
#   mat_mul(A, B, mod=None), mat_pow(A, e, mod=None), identity(n)
#   fib(n, mod=None)                        F(0) = 0, F(1) = 1
#   lin_rec(c, init, n, mod=None)           a(i) = c[0] a(i-1) + ... + c[k-1] a(i-k), a(0..k-1) = init
# Complexity: mat_pow O(k^3 log e); fib O(log n); lin_rec O(k^3 log n).


def identity(n):
    return [[int(i == j) for j in range(n)] for i in range(n)]


def mat_mul(A, B, mod=None):
    Bt = list(zip(*B))
    if mod is None:
        return [[sum(a * b for a, b in zip(row, col)) for col in Bt] for row in A]
    return [[sum(a * b for a, b in zip(row, col)) % mod for col in Bt] for row in A]


def mat_pow(A, e, mod=None):
    R = identity(len(A))
    if mod is not None:
        R = [[x % mod for x in r] for r in R]
    while e:
        if e & 1:
            R = mat_mul(R, A, mod)
        A = mat_mul(A, A, mod)
        e >>= 1
    return R


def fib(n, mod=None):
    def go(k):  # (F(k), F(k+1))
        if k == 0:
            return 0, 1
        a, b = go(k >> 1)
        c, d = a * (2 * b - a), a * a + b * b
        if mod is not None:
            c, d = c % mod, d % mod
        return (d, c + d) if k & 1 else (c, d)

    f = go(n)[0]
    return f % mod if mod is not None else f


def lin_rec(c, init, n, mod=None):
    k = len(c)
    if n < k:
        return init[n] % mod if mod is not None else init[n]
    M = [list(c)] + [[int(j == i) for j in range(k)] for i in range(k - 1)]
    P = mat_pow(M, n - k + 1, mod)
    v = sum(P[0][j] * init[k - 1 - j] for j in range(k))
    return v % mod if mod is not None else v
