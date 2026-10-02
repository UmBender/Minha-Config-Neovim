# Title: Divisors and multiplicative functions
# Description: Divisors, d(n), sigma_k, Euler phi, Moebius for one n, and linear sieves of them up to n.
# Usage:
#   divisors(n)             sorted list
#   num_divisors(n), sigma(n, k=1), phi(n), mobius(n)      (via factor, any n >= 1)
#   phi_sieve(n), mobius_sieve(n), divisor_count_sieve(n), divisor_sum_sieve(n)
#                           lists indexed 0..n (index 0 is 0)
# Complexity: one n: factor(n) + O(number of divisors); sieves O(n) (divisor ones O(n log n)).
# Requires: nt/primes
import math


def divisors(n):
    ds = [1]
    for p, e in sorted(factor(n).items()):
        ds = [d * p**i for d in ds for i in range(e + 1)]
    return sorted(ds)


def num_divisors(n):
    return math.prod(e + 1 for e in factor(n).values())


def sigma(n, k=1):
    if k == 0:
        return num_divisors(n)
    return math.prod((p ** (k * (e + 1)) - 1) // (p**k - 1) for p, e in factor(n).items())


def phi(n):
    return math.prod((p - 1) * p ** (e - 1) for p, e in factor(n).items())


def mobius(n):
    f = factor(n)
    return 0 if any(e > 1 for e in f.values()) else (-1) ** len(f)


def phi_sieve(n):
    ph = list(range(n + 1))
    for i in range(2, n + 1):
        if ph[i] == i:  # prime
            for j in range(i, n + 1, i):
                ph[j] -= ph[j] // i
    return ph


def mobius_sieve(n):
    mu = [1] * (n + 1)
    if n >= 0:
        mu[0] = 0
    is_comp = bytearray(n + 1)
    for i in range(2, n + 1):
        if not is_comp[i]:
            for j in range(i, n + 1, i):
                if j > i:
                    is_comp[j] = 1
                mu[j] = -mu[j]
            for j in range(i * i, n + 1, i * i):
                mu[j] = 0
    return mu


def divisor_count_sieve(n):
    d = [0] * (n + 1)
    for i in range(1, n + 1):
        for j in range(i, n + 1, i):
            d[j] += 1
    return d


def divisor_sum_sieve(n):
    s = [0] * (n + 1)
    for i in range(1, n + 1):
        for j in range(i, n + 1, i):
            s[j] += i
    return s
