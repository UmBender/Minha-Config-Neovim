# Problem: Check a claimed formula on every small input: the number of inversions summed over all
#   permutations of n is n! * n * (n - 1) / 4; and count arrays of length 3 over [0, 2] with sum 3.
# Output:
#   True True True True True
#   7
from libtest import include
include("gen/exhaustive")
from math import factorial


def inversions(p):
    return sum(p[i] > p[j] for i in range(len(p)) for j in range(i + 1, len(p)))


print(*(sum(map(inversions, all_perms(n))) * 4 == factorial(n) * n * (n - 1) for n in range(1, 6)))
print(sum(1 for a in all_arrays(3, 0, 2) if sum(a) == 3))
