from libtest import include, check, check_eq, rnd
include("gen/graph")
from itertools import permutations


def connected(n, edges):
    p = list(range(n))

    def find(x):
        while p[x] != x:
            x = p[x]
        return x

    for e in edges:
        p[find(e[0])] = find(e[1])
    return len({find(i) for i in range(n)}) <= 1


def acyclic_brute(n, edges):
    for order in permutations(range(n)):
        pos = {v: i for i, v in enumerate(order)}
        if all(pos[u] < pos[v] for u, v in edges):
            return True
    return False


# rand_graph: simple by default
for _ in range(300):
    n = rnd.randint(1, 8)
    directed = rnd.random() < 0.5
    full = n * (n - 1) // (1 if directed else 2)
    m = rnd.randint(0, full)
    e = rand_graph(n, m, directed=directed)
    check_eq(len(e), m)
    check(all(0 <= u < n and 0 <= v < n and u != v for u, v in e), e)
    keys = [(u, v) if directed else (min(u, v), max(u, v)) for u, v in e]
    check_eq(len(set(keys)), m)
    if m >= n - 1 and not directed:
        c = rand_graph(n, m, connected=True)
        check(connected(n, c) and len(c) == m and len({(min(u, v), max(u, v)) for u, v in c}) == m, c)
check_eq(sorted(map(sorted, rand_graph(3, 3))), [[0, 1], [0, 2], [1, 2]])
loops = rand_graph(2, 3, loops=True)
check_eq(sorted(tuple(sorted(x)) for x in loops), [(0, 0), (0, 1), (1, 1)])
multi = rand_graph(2, 10, multi=True)
check(len(multi) == 10 and all(u != v for u, v in multi), multi)
check(len(rand_graph(1000, 5)) == 5)  # sparse on many vertices: no O(n^2) list
for bad in [dict(n=3, m=4), dict(n=3, m=7, directed=True), dict(n=4, m=2, connected=True)]:
    try:
        rand_graph(**bad)
        check(False, ("should raise", bad))
    except ValueError:
        pass

# rand_dag: acyclic, simple, connected on request, every DAG shape reachable
for _ in range(300):
    n = rnd.randint(1, 6)
    m = rnd.randint(0, n * (n - 1) // 2)
    e = rand_dag(n, m)
    check(len(e) == m and len(set(e)) == m and acyclic_brute(n, e), e)
    check_eq(is_dag(n, e), True)
    if m >= n - 1:
        check(connected(n, rand_dag(n, m, connected=True)))
seen = {tuple(sorted(rand_dag(3, 1))) for _ in range(500)}
check_eq(len(seen), 6)  # the edge's direction isn't biased by the labels
check_eq(is_dag(2, [(0, 1), (1, 0)]), False)
check_eq(is_dag(1, [(0, 0)]), False)
check_eq(is_dag(3, []), True)

# rand_bipartite
for _ in range(100):
    a, b = rnd.randint(1, 5), rnd.randint(1, 5)
    m = rnd.randint(0, a * b)
    e = rand_bipartite(a, b, m)
    check(len(set(e)) == m and all(0 <= u < a and 0 <= v < b for u, v in e), e)

# rand_functional
f = rand_functional(10)
check(len(f) == 10 and all(0 <= x < 10 for x in f))
for _ in range(200):
    f = rand_functional(10, loops=False)
    check(all(f[i] != i and 0 <= f[i] < 10 for i in range(10)), f)
check({x for _ in range(200) for x in rand_functional(3)} == {0, 1, 2})

# weighted
w = weighted([(0, 1), (1, 2)], 5, 5)
check_eq(w, [(0, 1, 5), (1, 2, 5)])

# is_connected
check_eq(is_connected(3, [(0, 1)]), False)
check_eq(is_connected(3, [(0, 1), (2, 1)]), True)
check_eq(is_connected(1, []), True)
