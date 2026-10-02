from libtest import include, check, check_eq
include("nt/cont-frac")
from fractions import Fraction
from math import isqrt

check_eq(sqrt_cf(2), (1, [2]))
check_eq(sqrt_cf(23), (4, [1, 3, 1, 8]))
check_eq(sqrt_cf(13), (3, [1, 1, 1, 1, 6]))
check_eq(sqrt_cf(16), (4, []))
check_eq(sqrt_cf(0), (0, []))
# Project Euler 64: 4 periods are odd for N <= 13
check_eq(sum(1 for n in range(2, 14) if len(sqrt_cf(n)[1]) % 2), 4)

c = list(convergents([1, 2, 2, 2, 2]))
check_eq(c, [(1, 1), (3, 2), (7, 5), (17, 12), (41, 29)])
check_eq(list(convergents([])), [])
check_eq(list(convergents([3, 7, 15, 1]))[-1], (355, 113))
# convergents equal the truncated fraction
terms = [2, 1, 2, 1, 1, 4, 1, 1, 6]
for i, (p, q) in enumerate(convergents(terms)):
    f = Fraction(terms[i])
    for t in reversed(terms[:i]):
        f = t + 1 / f
    check_eq(Fraction(p, q), f)

# Pell: minimal solution vs brute force
for D in range(2, 70):
    if isqrt(D) ** 2 == D:
        check_eq(pell(D), None)
        continue
    x, y = pell(D)
    check_eq(x * x - D * y * y, 1)
    if y < 2000:
        yy = next(y for y in range(1, 2000) if isqrt(1 + D * y * y) ** 2 == 1 + D * y * y)
        check_eq(y, yy, D)
check_eq(pell(61), (1766319049, 226153980))
sols = pell_solutions(2)
check_eq([next(sols) for _ in range(4)], [(3, 2), (17, 12), (99, 70), (577, 408)])
