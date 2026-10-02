from libtest import include, check, check_eq, rnd
include("gen/tree")
import random
from collections import Counter


def is_tree(n, edges):
    if len(edges) != max(n - 1, 0):
        return False
    p = list(range(n))

    def find(x):
        while p[x] != x:
            p[x] = p[p[x]]
            x = p[x]
        return x

    for u, v in edges:
        if not (0 <= u < n and 0 <= v < n):
            return False
        a, b = find(u), find(v)
        if a == b:
            return False
        p[a] = b
    return True


def degrees(n, edges):
    d = [0] * n
    for u, v in edges:
        d[u] += 1
        d[v] += 1
    return d


def diameter(n, edges):
    if n == 1:
        return 0
    adj = [[] for _ in range(n)]
    for u, v in edges:
        adj[u].append(v)
        adj[v].append(u)

    def far(s):
        dist = [-1] * n
        dist[s] = 0
        q = [s]
        for x in q:
            for y in adj[x]:
                if dist[y] < 0:
                    dist[y] = dist[x] + 1
                    q.append(y)
        far_v = max(range(n), key=lambda i: dist[i])
        return far_v, dist[far_v]

    return far(far(0)[0])[1]


# every kind gives a tree for every n, shuffled or not
for kind in TREE_KINDS:
    for n in list(range(1, 30)) + [200]:
        for shuffle in (True, False):
            e = rand_tree(n, kind, shuffle=shuffle)
            check(is_tree(n, e), (kind, n, e))
            if not shuffle:
                check(all(u < v for u, v in e), ("parent < child when not shuffled", kind, e))

# shapes
n = 50
check_eq(diameter(n, rand_tree(n, "path")), n - 1)
check_eq(max(degrees(n, rand_tree(n, "star"))), n - 1)
check_eq(diameter(n, rand_tree(n, "star")), 2)
d = degrees(n, rand_tree(n, "binary", shuffle=False))
check(max(d) <= 3 and Counter(d)[3] >= 20, d)
check_eq(diameter(n, rand_tree(n, "double-star")), 3)
spider = rand_tree(49, "spider", shuffle=False)
check_eq(max(degrees(49, spider)), 6)  # sqrt(48) ~ 6 legs of 8
broom = rand_tree(n, "broom")
check(diameter(n, broom) >= n // 2 and max(degrees(n, broom)) >= n // 2 - 1, broom)
cat = rand_tree(n, "caterpillar")
check(diameter(n, cat) >= n // 2 - 1, cat)  # every vertex is on or next to the spine
check(diameter(1000, rand_tree(1000, "deep")) >= 200)
check(diameter(1000, rand_tree(1000, "shallow")) <= 60)
try:
    rand_tree(5, "nope")
    check(False, "unknown kind should raise")
except ValueError:
    pass

# Prüfer "random" is uniform: all 16 labeled trees on 4 vertices show up
seen = {tuple(sorted(tuple(sorted(e)) for e in rand_tree(4))) for _ in range(2000)}
check_eq(len(seen), 16)

# edge_case_trees: one per kind
cases = edge_case_trees(10)
check_eq([k for k, _ in cases], list(TREE_KINDS))
check(all(is_tree(10, e) for _, e in cases))

# relabel keeps the shape
for _ in range(50):
    n = rnd.randint(1, 20)
    e = rand_tree(n, "random", shuffle=False)
    r = relabel(n, e)
    check(is_tree(n, r))
    check_eq(sorted(degrees(n, r)), sorted(degrees(n, e)))
    check_eq(diameter(n, r), diameter(n, e))

# to_parents
check_eq(to_parents(1, []), [-1])
check_eq(to_parents(4, [(1, 0), (2, 1), (1, 3)]), [-1, 0, 1, 1])
check_eq(to_parents(4, [(1, 0), (2, 1), (1, 3)], root=2), [1, 2, -1, 1])
for _ in range(50):
    n = rnd.randint(1, 30)
    e = rand_tree(n)
    root = rnd.randrange(n)
    p = to_parents(n, e, root)
    check_eq(p[root], -1)
    check_eq(sorted(tuple(sorted((i, p[i]))) for i in range(n) if i != root), sorted(tuple(sorted(x)) for x in e))

# all_trees: Cayley's n^(n-2), all distinct, all trees
for n in range(1, 7):
    ts = list(all_trees(n))
    check_eq(len(ts), n ** (n - 2) if n >= 2 else 1)
    check(all(is_tree(n, t) for t in ts))
    check_eq(len({tuple(sorted(tuple(sorted(x)) for x in t)) for t in ts}), len(ts))

# prufer_decode inverts the encoding of a known tree: path 0-1-2-3 has sequence [1, 2]
check_eq(sorted(tuple(sorted(x)) for x in prufer_decode([1, 2])), [(0, 1), (1, 2), (2, 3)])
