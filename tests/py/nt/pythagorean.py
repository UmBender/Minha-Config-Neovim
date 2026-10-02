from libtest import include, check, check_eq
include("nt/pythagorean")
from math import gcd, isqrt


def brute(limit, key, primitive):
    out = set()
    for a in range(1, limit + 1):
        for b in range(a + 1, limit + 1):
            c2 = a * a + b * b
            c = isqrt(c2)
            if c * c != c2:
                continue
            k = c if key == "c" else a + b + c
            if k <= limit and (not primitive or gcd(a, b) == 1):
                out.add((a, b, c))
    return out


for limit in (0, 5, 13, 30, 100, 300):
    for key in ("c", "perimeter"):
        got = list(primitive_triples(limit, key))
        check_eq(len(got), len(set(got)))
        check_eq(set(got), brute(limit, key, True), (limit, key))
        got = list(triples(limit, key))
        check_eq(len(got), len(set(got)))
        check_eq(set(got), brute(limit, key, False), (limit, key))
check(all(a < b for a, b, _ in triples(100)))
# Project Euler 9: the only triple with a + b + c = 1000
check_eq([a * b * c for a, b, c in triples(1000, "perimeter") if a + b + c == 1000], [31875000])
