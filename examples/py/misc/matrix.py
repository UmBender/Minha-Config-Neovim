# Problem: F(10^18) mod 10^9 + 7; the number of 1x1 / 1x2 tilings of a 2 x n board is F(n + 1);
#   tribonacci T(n) = T(n-1) + T(n-2) + T(n-3) with T(0..2) = 0, 0, 1, at n = 37.
# Input:
#   1000000000000000000
# Output:
#   209783453
#   89
#   1132436852
from libtest import include
include("misc/matrix")

n = int(input())
print(fib(n, 10**9 + 7))
print(fib(10 + 1))
print(lin_rec([1, 1, 1], [0, 0, 1], 37))
