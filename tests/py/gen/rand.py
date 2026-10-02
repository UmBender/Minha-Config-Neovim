from libtest import include, check, check_eq, rnd
include("gen/rand")
import random

# rand_array: bounds and length
for _ in range(200):
    n, lo = rnd.randint(0, 20), rnd.randint(-5, 5)
    hi = lo + rnd.randint(0, 5)
    a = rand_array(n, lo, hi)
    check(len(a) == n and all(lo <= x <= hi for x in a), a)
check_eq(rand_array(3, 7, 7), [7, 7, 7])

# rand_distinct: distinct, in range, too many raises
for _ in range(200):
    lo = rnd.randint(-10, 10)
    hi = lo + rnd.randint(0, 15)
    n = rnd.randint(0, hi - lo + 1)
    a = rand_distinct(n, lo, hi)
    check(len(set(a)) == n and all(lo <= x <= hi for x in a), a)
check_eq(sorted(rand_distinct(5, 1, 5)), [1, 2, 3, 4, 5])
check_eq(rand_distinct(3, 0, 10**18) != [], True)  # huge range doesn't build a list
try:
    rand_distinct(4, 1, 3)
    check(False, "rand_distinct should raise")
except ValueError:
    pass

# rand_perm: a permutation of start..start+n-1, every one reachable
for n in range(6):
    check_eq(sorted(rand_perm(n)), list(range(n)))
    check_eq(sorted(rand_perm(n, 1)), list(range(1, n + 1)))
seen = {tuple(rand_perm(3)) for _ in range(300)}
check_eq(len(seen), 6)

# rand_string
s = rand_string(50, "xyz")
check(len(s) == 50 and set(s) <= set("xyz"), s)
check_eq(rand_string(0), "")
check(set(rand_string(100)) <= set("ab"))

# rand_partition: k parts >= min_part summing to total; all compositions reachable
for _ in range(300):
    k = rnd.randint(1, 6)
    mn = rnd.randint(0, 3)
    total = k * mn + rnd.randint(0, 10)
    p = rand_partition(total, k, mn)
    check(len(p) == k and sum(p) == total and min(p) >= mn, (total, k, mn, p))
seen = {tuple(rand_partition(3, 2)) for _ in range(300)}
check_eq(seen, {(0, 3), (1, 2), (2, 1), (3, 0)})
check_eq(rand_partition(0, 0), [])
try:
    rand_partition(5, 3, 2)
    check(False, "rand_partition should raise")
except ValueError:
    pass

# rand_subset
items = list(range(10))
for _ in range(100):
    s = rand_subset(items)
    check(s == sorted(s) and set(s) <= set(items), s)  # keeps the original order
    s = rand_subset(items, 4)
    check(len(s) == 4 and s == sorted(s), s)
check({len(rand_subset(items)) for _ in range(200)} > {4, 5, 6})

# fmt_edges / fmt
check_eq(fmt_edges([(0, 1), (1, 2)]), "1 2\n2 3")
check_eq(fmt_edges([(0, 1, 5)], base=0), "0 1 5")
check_eq(fmt_edges([]), "")
check_eq(fmt([1, 2, 3]), "1 2 3")
check_eq(fmt([]), "")

# reproducible through random.seed (the generator template seeds from argv)
random.seed(7)
a = (rand_array(5, 1, 100), rand_perm(5), rand_string(5))
random.seed(7)
check_eq((rand_array(5, 1, 100), rand_perm(5), rand_string(5)), a)
