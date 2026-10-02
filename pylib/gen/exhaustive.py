# Title: Exhaustive small inputs
# Description: Every array, permutation, subset, string, partition, composition, graph or DAG of a small size.
# Usage:
#   for a in all_arrays(n, lo, hi): ...       lists, (hi-lo+1)^n of them
#   for p in all_perms(n, start=0): ...       lists, lexicographic
#   for s in all_subsets(items): ...          lists, by size then order
#   for s in all_strings(n, "ab"): ...
#   for p in all_partitions(n): ...           non-increasing positive parts, [4], [3, 1], [2, 2], ...
#   for c in all_compositions(n, k=None, min_part=1): ...   ordered parts (any count when k is None)
#   for e in all_graphs(n, directed=False): ...   every edge subset, edges [(u, v)] with u < v if undirected
#   for e in all_dags(n): ...                 every labeled DAG (1, 1, 3, 25, 543, 29281)
# Complexity: proportional to the number of objects.
import itertools


def all_arrays(n, lo, hi):
    for t in itertools.product(range(lo, hi + 1), repeat=n):
        yield list(t)


def all_perms(n, start=0):
    for p in itertools.permutations(range(start, start + n)):
        yield list(p)


def all_subsets(items):
    items = list(items)
    for k in range(len(items) + 1):
        for c in itertools.combinations(items, k):
            yield list(c)


def all_strings(n, alphabet="ab"):
    for t in itertools.product(alphabet, repeat=n):
        yield "".join(t)


def all_partitions(n, max_part=None):
    max_part = n if max_part is None else max_part
    if n == 0:
        yield []
        return
    for first in range(min(n, max_part), 0, -1):
        for rest in all_partitions(n - first, first):
            yield [first] + rest


def all_compositions(n, k=None, min_part=1):
    if k is None:
        for k in range(0 if n == 0 else 1, n + 1):
            yield from all_compositions(n, k, min_part)
        return
    if k == 0:
        if n == 0:
            yield []
        return
    for first in range(min_part, n - (k - 1) * min_part + 1):
        for rest in all_compositions(n - first, k - 1, min_part):
            yield [first] + rest


def all_graphs(n, directed=False):
    pairs = [(u, v) for u in range(n) for v in range(n) if u != v and (directed or u < v)]
    for mask in range(1 << len(pairs)):
        yield [pairs[i] for i in range(len(pairs)) if mask >> i & 1]


def all_dags(n):
    for edges in all_graphs(n, directed=True):
        indeg = [0] * n
        for _, v in edges:
            indeg[v] += 1
        stack = [v for v in range(n) if indeg[v] == 0]
        seen = 0
        while stack:  # Kahn: acyclic iff every vertex gets removed
            v = stack.pop()
            seen += 1
            for a, b in edges:
                if a == v:
                    indeg[b] -= 1
                    if indeg[b] == 0:
                        stack.append(b)
        if seen == n:
            yield edges
