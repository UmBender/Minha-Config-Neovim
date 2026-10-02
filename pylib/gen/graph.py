# Title: Random graphs and DAGs
# Description: Random simple / connected / directed graphs, DAGs, bipartite and functional graphs, weights.
# Usage:
#   e = rand_graph(n, m)                         simple undirected, edges [(u, v)], 0-indexed
#   e = rand_graph(n, m, connected=True)         (m >= n - 1)
#   e = rand_graph(n, m, directed=True, loops=False, multi=False)
#   e = rand_dag(n, m, connected=False)          edges agree with a random topological order
#   e = rand_bipartite(n1, n2, m)                (u, v) with u in [0, n1), v in [0, n2)
#   f = rand_functional(n, loops=True)           f[i] in [0, n)
#   weighted(e, lo, hi) -> [(u, v, w)]
#   is_dag(n, e), is_connected(n, e)             checks for brute forces / asserts
#   print(n, len(e)); print(fmt_edges(e))        (gen/rand)
# Complexity: O(n + m) expected (O(n^2) when m is close to the maximum).
# Requires: gen/rand gen/tree
import random


def _rand_edges(n, m, directed, loops, multi, start=()):
    """`start` plus random new edges up to m, no repeats unless multi."""
    def key(u, v):
        return (u, v) if directed else (min(u, v), max(u, v))

    edges = list(start)
    if multi:
        while len(edges) < m:
            u, v = random.randrange(n), random.randrange(n)
            if u != v or loops:
                edges.append((u, v))
        return edges
    full = n * (n - 1) // (1 if directed else 2) + (n if loops else 0)
    if m > full:
        raise ValueError(f"{m} edges don't fit in a simple graph on {n} vertices")
    used = {key(u, v) for u, v in edges}
    if 2 * m > full:  # dense: sample from the full list
        pairs = [(u, v) for u in range(n) for v in range(n) if (u != v or loops) and (directed or u <= v)]
        rest = [p for p in pairs if p not in used]
        edges += random.sample(rest, m - len(edges))
        edges = [(v, u) if not directed and random.random() < 0.5 else (u, v) for u, v in edges]
    else:
        while len(edges) < m:
            u, v = random.randrange(n), random.randrange(n)
            if (u != v or loops) and key(u, v) not in used:
                used.add(key(u, v))
                edges.append((u, v))
    random.shuffle(edges)
    return edges


def rand_graph(n, m, connected=False, directed=False, loops=False, multi=False):
    start = ()
    if connected:
        if m < n - 1:
            raise ValueError(f"a connected graph on {n} vertices needs at least {n - 1} edges")
        start = rand_tree(n)
    return _rand_edges(n, m, directed, loops, multi, start)


def rand_dag(n, m, connected=False):
    order = list(range(n))
    random.shuffle(order)
    start = rand_tree(n) if connected else ()
    if connected and m < n - 1:
        raise ValueError(f"a connected DAG on {n} vertices needs at least {n - 1} edges")
    edges = _rand_edges(n, m, False, False, False, start)
    pos = [0] * n
    for i, v in enumerate(order):
        pos[v] = i
    return [(u, v) if pos[u] < pos[v] else (v, u) for u, v in edges]


def rand_bipartite(n1, n2, m):
    if m > n1 * n2:
        raise ValueError(f"{m} edges don't fit in a {n1}x{n2} bipartite graph")
    return [divmod(x, n2) for x in random.sample(range(n1 * n2), m)]


def rand_functional(n, loops=True):
    if loops:
        return [random.randrange(n) for _ in range(n)]
    return [(i + random.randrange(1, n)) % n for i in range(n)]


def weighted(edges, lo, hi):
    return [(u, v, random.randint(lo, hi)) for u, v, *_ in edges]


def is_dag(n, edges):
    indeg = [0] * n
    adj = [[] for _ in range(n)]
    for u, v, *_ in edges:
        adj[u].append(v)
        indeg[v] += 1
    stack = [v for v in range(n) if indeg[v] == 0]
    seen = 0
    while stack:
        v = stack.pop()
        seen += 1
        for w in adj[v]:
            indeg[w] -= 1
            if indeg[w] == 0:
                stack.append(w)
    return seen == n


def is_connected(n, edges):
    p = list(range(n))

    def find(x):
        while p[x] != x:
            p[x] = p[p[x]]
            x = p[x]
        return x

    comps = n
    for u, v, *_ in edges:
        a, b = find(u), find(v)
        if a != b:
            p[a] = b
            comps -= 1
    return comps <= 1
