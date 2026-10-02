from libtest import include, check, check_eq
include("nt/divisors")
from math import gcd

N = 1500
for n in range(1, N):
    ds = [d for d in range(1, n + 1) if n % d == 0]
    check_eq(divisors(n), ds, n)
    check_eq(num_divisors(n), len(ds))
    check_eq(sigma(n), sum(ds))
    check_eq(sigma(n, 0), len(ds))
    check_eq(sigma(n, 2), sum(d * d for d in ds))
    check_eq(phi(n), sum(1 for k in range(1, n + 1) if gcd(k, n) == 1), n)
    sq = any(n % (p * p) == 0 for p in range(2, n + 1))
    check_eq(mobius(n), 0 if sq else (-1) ** sum(1 for p in range(2, n + 1) if n % p == 0 and all(p % q for q in range(2, p))), n)
check_eq(divisors(1), [1])
check_eq(divisors(10**12), sorted(set(2**a * 5**b for a in range(13) for b in range(13))))
check_eq(phi(10**18 + 9), 10**18 + 8)

ps, ms, dc, ds = phi_sieve(N), mobius_sieve(N), divisor_count_sieve(N), divisor_sum_sieve(N)
check_eq(len(ps), N + 1)
for n in range(1, N + 1):
    check_eq((ps[n], ms[n], dc[n], ds[n]), (phi(n), mobius(n), num_divisors(n), sigma(n)), n)
check_eq(ps[0], 0)
