# Title: Modular arithmetic
# Description: Extended gcd, modular inverse, CRT for any moduli, binomials mod a prime.
# Usage:
#   g, x, y = ext_gcd(a, b)     a*x + b*y == g == gcd(a, b)
#   inv_mod(a, m)               inverse in [0, m), None if gcd(a, m) != 1
#   crt(rs, ms)                 (r, lcm) with x == r (mod lcm) for all x % m_i == r_i, None if inconsistent
#   B = Binom(n, mod)           factorials up to n, mod a prime > n
#   B.C(n, k), B.P(n, k), B.fact[i], B.inv_fact[i], B.inv(i)    (C/P are 0 outside 0 <= k <= n)
#   Builtins: pow(a, -1, m), pow(b, e, m), math.comb, math.lcm.
# Complexity: ext_gcd / inv_mod O(log m); crt O(k log); Binom O(n) build, O(1) per query.
import math


def ext_gcd(a, b):
    x0, y0, x1, y1 = 1, 0, 0, 1
    while b:
        q = a // b
        a, b = b, a - q * b
        x0, x1 = x1, x0 - q * x1
        y0, y1 = y1, y0 - q * y1
    if a < 0:
        a, x0, y0 = -a, -x0, -y0
    return a, x0, y0


def inv_mod(a, m):
    g, x, _ = ext_gcd(a % m, m)
    return x % m if g == 1 else None


def crt(rs, ms):
    r, L = 0, 1
    for ri, mi in zip(rs, ms):
        g, p, _ = ext_gcd(L, mi)
        if (ri - r) % g:
            return None
        r += L * ((ri - r) // g * p % (mi // g))
        L = L // g * mi
        r %= L
    return r, L


class Binom:
    def __init__(self, n, mod):
        self.mod = mod
        self.fact = [1] * (n + 1)
        for i in range(1, n + 1):
            self.fact[i] = self.fact[i - 1] * i % mod
        self.inv_fact = [1] * (n + 1)
        self.inv_fact[n] = pow(self.fact[n], -1, mod)
        for i in range(n, 0, -1):
            self.inv_fact[i - 1] = self.inv_fact[i] * i % mod

    def C(self, n, k):
        if k < 0 or n < 0 or k > n:
            return 0
        return self.fact[n] * self.inv_fact[k] % self.mod * self.inv_fact[n - k] % self.mod

    def P(self, n, k):
        if k < 0 or n < 0 or k > n:
            return 0
        return self.fact[n] * self.inv_fact[n - k] % self.mod

    def inv(self, i):
        return self.inv_fact[i] * self.fact[i - 1] % self.mod
