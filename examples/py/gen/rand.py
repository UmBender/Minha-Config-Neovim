# Problem: Generator for "t tests, each an array of n values in [1, 10^9]; the sum of n over all tests is
#   exactly 20". Print the test count and the size of each test (the arrays themselves vary with the seed).
# Output:
#   3
#   20 True
from libtest import include
include("gen/rand")
import random

random.seed(1)
t = 3
sizes = rand_partition(20, t, min_part=1)  # every test has n >= 1
tests = [rand_array(n, 1, 10**9) for n in sizes]
print(t)
print(sum(len(a) for a in tests), all(1 <= x <= 10**9 for a in tests for x in a))
# in a real generator:
# print(t)
# for a in tests:
#     print(len(a))
#     print(fmt(a))
