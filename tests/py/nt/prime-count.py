from libtest import include, check, check_eq, rnd
include("nt/prime-count")


def sieve_list(n):
    s = [True] * (n + 1)
    s[0:2] = [False] * min(2, n + 1)
    for i in range(2, int(n**0.5) + 1):
        if s[i]:
            s[i * i :: i] = [False] * len(s[i * i :: i])
    return [i for i in range(n + 1) if s[i]]


ps = sieve_list(20000)
for n in list(range(0, 300)) + [rnd.randint(300, 20000) for _ in range(100)]:
    want = [p for p in ps if p <= n]
    check_eq(prime_pi(n), len(want), n)
    check_eq(prime_sum(n), sum(want), n)
check_eq(prime_pi(10**9), 50847534)
check_eq(prime_sum(2 * 10**6), 142913828922)  # Project Euler 10
