# Title: Pythagorean triples
# Description: Primitive and all Pythagorean triples (a < b < c) up to a bound on c or on the perimeter.
# Usage:
#   for a, b, c in primitive_triples(limit): ...            c <= limit, gcd(a, b) == 1
#   for a, b, c in triples(limit, key="perimeter"): ...     all triples with a + b + c <= limit
# Complexity: O(number of triples) after an O(limit^(1/2)) x O(limit^(1/2)) scan of (m, n).
import math


def primitive_triples(limit, key="c"):
    m = 2
    while (m * m + 1 if key == "c" else 2 * m * (m + 1)) <= limit:
        for n in range(1 + m % 2, m, 2):  # Euclid: m > n, opposite parity, coprime
            if math.gcd(m, n) != 1:
                continue
            a, b, c = m * m - n * n, 2 * m * n, m * m + n * n
            if (c if key == "c" else a + b + c) > limit:
                break
            yield (a, b, c) if a < b else (b, a, c)
        m += 1


def triples(limit, key="c"):
    for a, b, c in primitive_triples(limit, key):
        size = c if key == "c" else a + b + c
        for k in range(1, limit // size + 1):
            yield k * a, k * b, k * c
