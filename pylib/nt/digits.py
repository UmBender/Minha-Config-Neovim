# Title: Digits and integer roots
# Description: Digits in any base, digit sums, palindromes, exact integer k-th roots and square test.
# Usage:
#   digits(1203) == [1, 2, 0, 3]; digits(10, 2) == [1, 0, 1, 0]; digits(0) == [0]
#   from_digits(ds, base=10), digit_sum(n, base=10), num_len(n) (decimal length)
#   is_palindrome(x, base=10)     int (in base), str or list
#   iroot(n, k)                   floor(n^(1/k)), exact for big ints (math.isqrt for k = 2)
#   is_square(n)
# Complexity: O(number of digits); iroot O(log n) Newton steps.
import math


def digits(n, base=10):
    if n == 0:
        return [0]
    ds = []
    while n:
        n, d = divmod(n, base)
        ds.append(d)
    return ds[::-1]


def from_digits(ds, base=10):
    n = 0
    for d in ds:
        n = n * base + d
    return n


def digit_sum(n, base=10):
    if base == 10:
        return sum(map(int, str(n)))
    return sum(digits(n, base))


def num_len(n):
    return len(str(n))


def is_palindrome(x, base=10):
    s = (str(x) if base == 10 else digits(x, base)) if isinstance(x, int) else x
    return list(s) == list(s)[::-1]


def iroot(n, k):
    if k == 1 or n < 2:
        return n
    if k == 2:
        return math.isqrt(n)
    r = 1 << ((n.bit_length() + k - 1) // k)  # >= the root; Newton decreases to it
    while True:
        s = ((k - 1) * r + n // r ** (k - 1)) // k
        if s >= r:
            return r
        r = s


def is_square(n):
    return n >= 0 and math.isqrt(n) ** 2 == n
