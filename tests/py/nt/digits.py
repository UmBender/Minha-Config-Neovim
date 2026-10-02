from libtest import include, check, check_eq, rnd
include("nt/digits")
from math import isqrt

check_eq(digits(0), [0])
check_eq(digits(1203), [1, 2, 0, 3])
check_eq(digits(10, 2), [1, 0, 1, 0])
check_eq(digits(255, 16), [15, 15])
for _ in range(500):
    n, b = rnd.randint(0, 10**30), rnd.randint(2, 40)
    check_eq(from_digits(digits(n, b), b), n)
    check_eq(digit_sum(n, b), sum(digits(n, b)))
    check_eq(num_len(n), len(str(n)))
check_eq(from_digits([]), 0)
check_eq(digit_sum(2**15), 26)
check_eq(is_palindrome(12321), True)
check_eq(is_palindrome(10), False)
check_eq(is_palindrome("abba"), True)
check_eq(is_palindrome([1, 2]), False)
check_eq(is_palindrome(0), True)
check_eq(is_palindrome(5, 2), True)  # 101
check_eq(is_palindrome(6, 2), False)

for n in list(range(0, 3000)) + [rnd.randint(0, 10**40) for _ in range(300)]:
    for k in (1, 2, 3, 5):
        r = iroot(n, k)
        check(r**k <= n < (r + 1) ** k, (n, k, r))
    check_eq(is_square(n), isqrt(n) ** 2 == n)
check_eq(iroot(10**30, 3), 10**10)
check_eq(iroot(10**30 - 1, 3), 10**10 - 1)
check_eq(iroot(2**200, 50), 16)
check_eq(is_square(-4), False)
