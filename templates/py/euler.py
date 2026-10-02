# Project Euler {{N}}
# https://projecteuler.net/problem={{N}}
import sys
import time
from collections import Counter, defaultdict
from fractions import Fraction
from functools import cache
from itertools import combinations, count, permutations, product
from math import comb, factorial, gcd, isqrt, lcm, prod

sys.setrecursionlimit(1 << 20)
sys.set_int_max_str_digits(0)


def solve(n):
    return 0


def main():
    # check the statement's small case first, e.g. assert solve(10) == 23
    t = time.perf_counter()
    print(solve(1000))
    print(f"{time.perf_counter() - t:.3f}s", file=sys.stderr)


if __name__ == "__main__":
    main()
