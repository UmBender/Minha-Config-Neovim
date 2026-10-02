import sys
from collections import Counter, defaultdict, deque
from functools import cache
from itertools import accumulate, combinations, permutations, product
from math import comb, gcd, inf, isqrt, lcm

sys.setrecursionlimit(1 << 20)


def tokens():
    for line in sys.stdin:  # line by line, so typing the input in a terminal works
        yield from line.split()


_tok = tokens()


def ns():
    return next(_tok)


def ni():
    return int(next(_tok))


def nl(k):
    return [int(next(_tok)) for _ in range(k)]


def solve():
    


def main():
    t = ni()
    for _ in range(t):
        solve()


if __name__ == "__main__":
    main()
