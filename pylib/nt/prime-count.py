# Title: Prime counting
# Description: pi(n) and the sum of primes <= n without sieving to n (Lucy_Hedgehog).
# Usage:
#   prime_pi(10**11)       number of primes <= n
#   prime_sum(2 * 10**6)   sum of primes <= n
# Complexity: O(n^(3/4)) time, O(sqrt n) memory (n = 1e11 takes a few seconds in CPython).
import math


def _lucy(n, f):
    """S[v] = sum of g(p) over primes p <= v, for every v = n // i; f(v) = sum of g(i) for 2 <= i <= v."""
    r = math.isqrt(n)
    vals = [n // i for i in range(1, r + 1)]
    vals += list(range(vals[-1] - 1, 0, -1)) if vals else []
    S = {v: f(v) for v in vals}
    for p in range(2, r + 1):
        if S[p] > S[p - 1]:  # p is prime
            sp, gp, p2 = S[p - 1], S[p] - S[p - 1], p * p
            for v in vals:
                if v < p2:
                    break
                S[v] -= gp * (S[v // p] - sp)
    return S[n] if n >= 1 else 0


def prime_pi(n):
    return _lucy(n, lambda v: v - 1)


def prime_sum(n):
    return _lucy(n, lambda v: v * (v + 1) // 2 - 1)
