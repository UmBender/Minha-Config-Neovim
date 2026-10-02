# Problem: Project Euler 10: the sum of the primes below two million; and pi(10^10).
# Output:
#   142913828922
#   455052511
from libtest import include
include("nt/prime-count")

print(prime_sum(2 * 10**6))
print(prime_pi(10**10))
