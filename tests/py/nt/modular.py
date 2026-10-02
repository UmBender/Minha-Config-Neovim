from libtest import include, check, check_eq, rnd
include("nt/modular")
from math import gcd, comb, perm

for _ in range(2000):
    a, b = rnd.randint(-10**6, 10**6), rnd.randint(-10**6, 10**6)
    g, x, y = ext_gcd(a, b)
    check_eq(g, gcd(a, b))
    check_eq(a * x + b * y, g)
check_eq(ext_gcd(0, 0), (0, 1, 0))
check_eq(ext_gcd(5, 0)[0], 5)

for m in range(1, 60):
    for a in range(-60, 60):
        r = inv_mod(a, m)
        if gcd(a, m) == 1:
            check(r is not None and 0 <= r < m and (a * r - 1) % m == 0, (a, m, r))
        else:
            check_eq(r, None, (a, m))


def brute_crt(rs, ms):
    L = 1
    for m in ms:
        L = L * m // gcd(L, m)
    for x in range(L):
        if all(x % m == r % m for r, m in zip(rs, ms)):
            return (x, L)
    return None


for _ in range(1500):
    k = rnd.randint(1, 3)
    ms = [rnd.randint(1, 12) for _ in range(k)]
    rs = [rnd.randint(-20, 20) for _ in range(k)]
    check_eq(crt(rs, ms), brute_crt(rs, ms), (rs, ms))
check_eq(crt([], []), (0, 1))
r, L = crt([2, 3], [10**18, 10**18 + 1])
check(r % 10**18 == 2 and r % (10**18 + 1) == 3 and L == 10**18 * (10**18 + 1))

for mod in (10**9 + 7, 998244353, 13):
    B = Binom(12, mod)
    for n in range(-1, 13):
        for k in range(-1, 14):
            want = comb(n, k) % mod if 0 <= k <= n else 0
            check_eq(B.C(n, k), want, (n, k))
            check_eq(B.P(n, k), perm(n, k) % mod if 0 <= k <= n else 0, (n, k))
    check_eq(B.fact[12], 479001600 % mod)
    check_eq(B.inv(5) * 5 % mod, 1)
