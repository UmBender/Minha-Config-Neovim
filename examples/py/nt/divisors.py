# Problem: Project Euler 12 and 21: the first triangle number with over 500 divisors, and the sum of
#   the amicable numbers under 10000.
# Output:
#   76576500
#   31626
from libtest import include
include("nt/divisors")
from itertools import count

for k in count(1):
    t = k * (k + 1) // 2
    if num_divisors(t) > 500:
        print(t)
        break
N = 10000
s = divisor_sum_sieve(3 * N)
d = [s[i] - i for i in range(N)]
print(sum(a for a in range(2, N) if d[a] != a and d[a] < len(s) and s[d[a]] - d[a] == a))
