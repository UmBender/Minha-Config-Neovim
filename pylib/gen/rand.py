# Title: Random basics
# Description: Random arrays, distinct values, permutations, strings, partitions, subsets; edge list printing.
# Usage:
#   rand_array(n, lo, hi)          n values in [lo, hi]
#   rand_distinct(n, lo, hi)       n distinct values in [lo, hi] (any range size)
#   rand_perm(n, start=0)          permutation of start..start+n-1
#   rand_string(n, alphabet="ab")
#   rand_partition(total, k, min_part=0)   k parts >= min_part summing to total (e.g. sum of n over tests)
#   rand_subset(items, k=None)     random subset (each item w.p. 1/2, or exactly k), original order
#   fmt(a) -> "1 2 3";  fmt_edges(edges, base=1) -> "u v [w]" lines, vertices shifted by base
#   All use the global `random`, so random.seed(seed) makes a test reproducible.
# Complexity: O(output size).
import random


def rand_array(n, lo, hi):
    return [random.randint(lo, hi) for _ in range(n)]


def rand_distinct(n, lo, hi):
    if n > hi - lo + 1:
        raise ValueError(f"can't pick {n} distinct values from [{lo}, {hi}]")
    return random.sample(range(lo, hi + 1), n)


def rand_perm(n, start=0):
    p = list(range(start, start + n))
    random.shuffle(p)
    return p


def rand_string(n, alphabet="ab"):
    return "".join(random.choices(alphabet, k=n))


def rand_partition(total, k, min_part=0):
    free = total - k * min_part
    if free < 0 or (k == 0 and total != 0):
        raise ValueError(f"can't split {total} into {k} parts >= {min_part}")
    if k == 0:
        return []
    cuts = sorted(random.sample(range(free + k - 1), k - 1))  # stars and bars: uniform over compositions
    bounds = [-1] + cuts + [free + k - 1]
    return [bounds[i + 1] - bounds[i] - 1 + min_part for i in range(k)]


def rand_subset(items, k=None):
    items = list(items)
    if k is None:
        return [x for x in items if random.random() < 0.5]
    keep = set(random.sample(range(len(items)), k))
    return [x for i, x in enumerate(items) if i in keep]


def fmt(a):
    return " ".join(map(str, a))


def fmt_edges(edges, base=1):
    return "\n".join(" ".join(map(str, (e[0] + base, e[1] + base, *e[2:]))) for e in edges)
