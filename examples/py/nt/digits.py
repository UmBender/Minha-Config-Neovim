# Problem: Project Euler 4, 16 and 36: the largest palindrome made from the product of two 3-digit
#   numbers, the digit sum of 2^1000, the sum of numbers below 10^6 palindromic in bases 10 and 2.
# Output:
#   906609
#   1366
#   872187
from libtest import include
include("nt/digits")

print(max(a * b for a in range(100, 1000) for b in range(a, 1000) if is_palindrome(a * b)))
print(digit_sum(2**1000))
print(sum(n for n in range(1, 10**6) if is_palindrome(n) and is_palindrome(n, 2)))
