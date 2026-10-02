# Title: Random trees
# Description: Random and edge-case trees (path, star, caterpillar, broom, spider, ...), Prüfer, all labeled trees.
# Usage:
#   e = rand_tree(n)                   uniform random labeled tree, edges [(u, v)], 0-indexed
#   e = rand_tree(n, "path")           kinds in TREE_KINDS: random path star caterpillar binary broom
#                                      spider double-star deep shallow
#   e = rand_tree(n, kind, shuffle=False)   canonical labels: root 0, edges (parent, child), parent < child
#   for kind, e in edge_case_trees(n): ...  one tree per kind
#   relabel(n, e)                      random labels, edge order and endpoint order
#   to_parents(n, e, root=0)           parent array, parent[root] = -1
#   for e in all_trees(n): ...         every labeled tree (n^(n-2)), for exhaustive brute checks
#   prufer_decode(seq)                 tree on len(seq) + 2 vertices
#   print(fmt_edges(e))                1-indexed "u v" lines (gen/rand)
# Complexity: O(n log n) per tree; all_trees O(n^(n-1)).
# Requires: gen/rand
import heapq
import itertools
import math
import random

TREE_KINDS = ("random", "path", "star", "caterpillar", "binary", "broom", "spider", "double-star", "deep", "shallow")


def prufer_decode(seq):
    n = len(seq) + 2
    deg = [1] * n
    for x in seq:
        deg[x] += 1
    leaves = [i for i in range(n) if deg[i] == 1]
    heapq.heapify(leaves)
    edges = []
    for x in seq:
        leaf = heapq.heappop(leaves)
        edges.append((x, leaf))
        deg[x] -= 1
        if deg[x] == 1:
            heapq.heappush(leaves, x)
    edges.append((heapq.heappop(leaves), heapq.heappop(leaves)))
    return edges


def _tree_parents(n, kind):
    """parent[i] < i for i >= 1 (canonical shape, root 0)."""
    par = [-1] * n
    if kind == "path":
        for i in range(1, n):
            par[i] = i - 1
    elif kind == "star":
        for i in range(1, n):
            par[i] = 0
    elif kind == "binary":
        for i in range(1, n):
            par[i] = (i - 1) // 2
    elif kind == "caterpillar":
        spine = max(1, n // 2)
        for i in range(1, n):
            par[i] = i - 1 if i < spine else random.randrange(spine)
    elif kind == "broom":
        handle = max(1, n // 2)
        for i in range(1, n):
            par[i] = i - 1 if i < handle else handle - 1
    elif kind == "spider":
        legs = max(1, math.isqrt(n - 1)) if n > 1 else 1
        length = max(1, (n - 1 + legs - 1) // legs)
        for i in range(1, n):
            par[i] = 0 if (i - 1) % length == 0 else i - 1
    elif kind == "double-star":
        for i in range(1, n):
            par[i] = 0 if i == 1 or i % 2 == 0 else 1
    elif kind == "deep":
        for i in range(1, n):
            par[i] = random.randint(max(0, i - 3), i - 1)
    elif kind == "shallow":
        for i in range(1, n):
            par[i] = random.randrange(i)
    else:
        raise ValueError(f"unknown tree kind {kind!r}, expected one of {TREE_KINDS}")
    return par


def relabel(n, edges):
    p = list(range(n))
    random.shuffle(p)
    out = [(p[u], p[v]) if random.random() < 0.5 else (p[v], p[u]) for u, v in edges]
    random.shuffle(out)
    return out


def rand_tree(n, kind="random", shuffle=True):
    if kind == "random":
        if n <= 2:
            edges = [(0, 1)] if n == 2 else []
        else:
            edges = prufer_decode([random.randrange(n) for _ in range(n - 2)])
        if not shuffle:  # canonical labels: BFS order from 0
            par = to_parents(n, edges)
            order = [0]
            children = [[] for _ in range(n)]
            for v in range(1, n):
                children[par[v]].append(v)
            for v in order:
                order += children[v]
            pos = {v: i for i, v in enumerate(order)}
            return sorted((pos[par[v]], pos[v]) for v in range(1, n))
        return relabel(n, edges)
    par = _tree_parents(n, kind)
    edges = [(par[i], i) for i in range(1, n)]
    return relabel(n, edges) if shuffle else edges


def edge_case_trees(n):
    return [(kind, rand_tree(n, kind)) for kind in TREE_KINDS]


def to_parents(n, edges, root=0):
    adj = [[] for _ in range(n)]
    for e in edges:
        adj[e[0]].append(e[1])
        adj[e[1]].append(e[0])
    par = [-1] * n
    seen = [False] * n
    seen[root] = True
    stack = [root]
    while stack:
        v = stack.pop()
        for w in adj[v]:
            if not seen[w]:
                seen[w] = True
                par[w] = v
                stack.append(w)
    return par


def all_trees(n):
    if n <= 2:
        yield [(0, 1)] if n == 2 else []
        return
    for seq in itertools.product(range(n), repeat=n - 2):
        yield prufer_decode(list(seq))
