# Title: Continued fractions and Pell
# Description: Periodic continued fraction of sqrt(n), convergents, Pell equation x^2 - D y^2 = 1.
# Usage:
#   a0, period = sqrt_cf(23)          (4, [1, 3, 1, 8]); perfect squares give (r, [])
#   for p, q in convergents([1, 2, 2, 2]): ...   p/q of each prefix: 1/1, 3/2, 7/5, 17/12
#   x, y = pell(D)                    minimal solution, None if D is a square
#   sols = pell_solutions(D); next(sols)   all solutions in increasing order
# Complexity: sqrt_cf O(period) = O(sqrt n log n); pell O(period) big-int steps.
import math


def sqrt_cf(n):
    a0 = math.isqrt(n)
    if a0 * a0 == n:
        return a0, []
    m, d, a, period = 0, 1, a0, []
    while a != 2 * a0:
        m = d * a - m
        d = (n - m * m) // d
        a = (a0 + m) // d
        period.append(a)
    return a0, period


def convergents(terms):
    p0, q0, p1, q1 = 1, 0, 0, 1  # p_{-1}/q_{-1}, p_{-2}/q_{-2}
    for a in terms:
        p0, p1 = a * p0 + p1, p0
        q0, q1 = a * q0 + q1, q0
        yield p0, q0


def pell(D):
    a0, period = sqrt_cf(D)
    if not period:
        return None
    k = len(period)
    terms = [a0] + (period * 2)[: (k if k % 2 == 0 else 2 * k) - 1]
    *_, (x, y) = convergents(terms)
    return x, y


def pell_solutions(D):
    x1, y1 = pell(D)
    x, y = x1, y1
    while True:
        yield x, y
        x, y = x1 * x + D * y1 * y, x1 * y + y1 * x
