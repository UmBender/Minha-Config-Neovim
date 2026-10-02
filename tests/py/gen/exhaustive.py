from libtest import include, check, check_eq
include("gen/exhaustive")
from math import comb, factorial

check_eq(list(all_arrays(2, 0, 1)), [[0, 0], [0, 1], [1, 0], [1, 1]])
check_eq(list(all_arrays(0, 1, 3)), [[]])
check_eq(len(list(all_arrays(3, -1, 1))), 27)
check_eq(list(all_perms(3))[:2], [[0, 1, 2], [0, 2, 1]])
check_eq(len(list(all_perms(5))), 120)
check_eq(list(all_perms(2, 1)), [[1, 2], [2, 1]])
check_eq(list(all_subsets([1, 2])), [[], [1], [2], [1, 2]])
check_eq(len(list(all_subsets(range(6)))), 64)
check_eq(list(all_strings(2, "ab")), ["aa", "ab", "ba", "bb"])

# integer partitions: p(n), non-increasing, sum n
P = [1, 1, 2, 3, 5, 7, 11, 15, 22, 30, 42]
for n in range(11):
    ps = list(all_partitions(n))
    check_eq(len(ps), P[n])
    check(all(sum(p) == n and p == sorted(p, reverse=True) for p in ps))
check_eq(list(all_partitions(4)), [[4], [3, 1], [2, 2], [2, 1, 1], [1, 1, 1, 1]])

# compositions: 2^(n-1) total, C(n-1, k-1) with k parts (positive), C(n+k-1, k-1) with zeros
for n in range(1, 9):
    check_eq(len(list(all_compositions(n))), 2 ** (n - 1))
    for k in range(1, n + 2):
        cs = list(all_compositions(n, k))
        check_eq(len(cs), comb(n - 1, k - 1))
        check(all(len(c) == k and sum(c) == n and min(c) > 0 for c in cs))
        zs = list(all_compositions(n, k, min_part=0))
        check_eq(len(zs), comb(n + k - 1, k - 1))
check_eq(list(all_compositions(0)), [[]])

# graphs
for n in range(5):
    check_eq(len(list(all_graphs(n))), 2 ** (n * (n - 1) // 2))
    check_eq(len(list(all_graphs(n, directed=True))), 2 ** (n * (n - 1)))
check_eq(list(all_graphs(2)), [[], [(0, 1)]])
# labeled DAGs: 1, 1, 3, 25, 543
check_eq([len(list(all_dags(n))) for n in range(5)], [1, 1, 3, 25, 543])
check_eq(len([g for g in all_dags(3) if len(g) == 3]), factorial(3))  # one per topological order
