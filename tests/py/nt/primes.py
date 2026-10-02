from libtest import include, check, check_eq, rnd
include("nt/primes")
from collections import Counter


def brute_prime(n):
    return n >= 2 and all(n % d for d in range(2, int(n**0.5) + 1))


N = 3000
s = sieve(N)
check_eq(len(s), N + 1)
check_eq([i for i in range(N + 1) if s[i]], [i for i in range(N + 1) if brute_prime(i)])
check_eq(primes_upto(30), [2, 3, 5, 7, 11, 13, 17, 19, 23, 29])
check_eq(primes_upto(1), [])
check_eq(primes_upto(2), [2])
check_eq(len(primes_upto(10**6)), 78498)
check_eq(list(sieve(1)), [0, 0])
check_eq(list(sieve(0)), [0])

spf = spf_sieve(N)
for i in range(2, N + 1):
    check_eq(spf[i], min(d for d in range(2, i + 1) if i % d == 0), i)
check_eq(spf[:2], [0, 1])

for n in range(-5, N):
    check_eq(is_prime(n), brute_prime(n), n)
big_primes = [2**61 - 1, 10**18 + 9, 2**89 - 1, 1000000007, 998244353, 2**127 - 1]
for p in big_primes:
    check(is_prime(p), p)
for c in [2**61 + 1, 10**18 + 7, 3215031751, 3825123056546413051, 318665857834031151167461,
          (2**31 - 1) * (2**61 - 1), 1000000007 * 998244353, 561, 41041, 2**64 + 1]:
    check(not is_prime(c), c)


def brute_factor(n):
    f, d = Counter(), 2
    while d * d <= n:
        while n % d == 0:
            f[d] += 1
            n //= d
        d += 1
    if n > 1:
        f[n] += 1
    return f


for n in range(1, 3000):
    check_eq(factor(n), brute_factor(n), n)
check_eq(factor(1), Counter())
check_eq(factor(0), Counter())
for _ in range(60):
    k = rnd.randint(1, 4)  # rho needs ~sqrt(p) steps: one huge prime at most, the rest <= 2^32
    ps = [rnd.choice([2, 3, 1009, 65537, 4294967291, 1000000007, 998244353]) for _ in range(k)]
    ps += [rnd.choice([2**61 - 1, 10**18 + 9, 2**89 - 1])] * rnd.randint(0, 1)
    n = 1
    for p in ps:
        n *= p
    check_eq(factor(n), Counter(ps), ps)
check_eq(factor(2**60), Counter({2: 60}))
check_eq(factor(600851475143), Counter({71: 1, 839: 1, 1471: 1, 6857: 1}))  # Project Euler 3
