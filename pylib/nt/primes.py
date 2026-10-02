# Title: Primes and factorization
# Description: Sieve, smallest prime factors, deterministic Miller-Rabin and Pollard rho factorization.
# Usage:
#   s = sieve(n)            bytearray, s[i] == 1 iff i is prime (0 <= i <= n)
#   ps = primes_upto(n)     [2, 3, 5, ...]
#   spf = spf_sieve(n)      smallest prime factor of 2..n (spf[0] = 0, spf[1] = 1)
#   is_prime(n)             any int; deterministic below 3.3e24, probabilistic above
#   factor(n)               Counter {p: e}, e.g. factor(12) == {2: 2, 3: 1}; factor(1) == {}
# Complexity: sieve O(n log log n); is_prime O(log^3 n); factor O(n^(1/4)) expected.
import math
import random
from collections import Counter


def sieve(n):
    s = bytearray([1]) * (n + 1)
    s[: min(2, n + 1)] = bytes(min(2, n + 1))
    for i in range(2, math.isqrt(n) + 1):
        if s[i]:
            s[i * i :: i] = bytes(len(range(i * i, n + 1, i)))
    return s


def primes_upto(n):
    return [i for i, p in enumerate(sieve(n)) if p]


def spf_sieve(n):
    spf = list(range(n + 1))
    for i in range(2, math.isqrt(n) + 1):
        if spf[i] == i:
            for j in range(i * i, n + 1, i):
                if spf[j] == j:
                    spf[j] = i
    return spf


def is_prime(n):
    if n < 2:
        return False
    small = (2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41)
    for p in small:
        if n % p == 0:
            return n == p
    d, r = n - 1, 0
    while d % 2 == 0:
        d //= 2
        r += 1
    for a in small:  # the first 13 primes are enough below 3.3e24
        x = pow(a, d, n)
        if x in (1, n - 1):
            continue
        for _ in range(r - 1):
            x = x * x % n
            if x == n - 1:
                break
        else:
            return False
    return True


def _pollard(n):
    if n % 2 == 0:
        return 2
    while True:  # Brent's cycle detection, gcd batched over 128 steps
        y, c, m = random.randrange(1, n), random.randrange(1, n), 128
        g = r = q = 1
        while g == 1:
            x = y
            for _ in range(r):
                y = (y * y + c) % n
            k = 0
            while k < r and g == 1:
                ys = y
                for _ in range(min(m, r - k)):
                    y = (y * y + c) % n
                    q = q * abs(x - y) % n
                g = math.gcd(q, n)
                k += m
            r *= 2
        if g == n:
            g = 1
            while g == 1:
                ys = (ys * ys + c) % n
                g = math.gcd(abs(x - ys), n)
        if g != n:
            return g


def factor(n):
    f = Counter()
    if n < 2:
        return f
    for p in (2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37):
        while n % p == 0:
            f[p] += 1
            n //= p
    stack = [n] if n > 1 else []
    while stack:
        m = stack.pop()
        if is_prime(m):
            f[m] += 1
        else:
            d = _pollard(m)
            stack += [d, m // d]
    return f
