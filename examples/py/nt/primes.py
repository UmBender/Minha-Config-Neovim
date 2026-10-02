# Problem: Project Euler 3 and 7: the largest prime factor of 600851475143 and the 10001st prime;
#   also whether 2^61 - 1 is prime.
# Output:
#   6857
#   104743
#   True
from libtest import include
include("nt/primes")

print(max(factor(600851475143)))
print(primes_upto(200000)[10000])
print(is_prime(2**61 - 1))
