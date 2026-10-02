# Problem: Print every edge-case tree kind on 7 vertices with its height from vertex 0, then a path in
#   the 1-indexed "u v" format a judge reads (canonical labels, so the output is fixed).
# Output:
#   random True
#   path 6
#   star 1
#   caterpillar True
#   binary 2
#   broom 3
#   spider 3
#   double-star 2
#   deep True
#   shallow True
#   1 2
#   2 3
#   3 4
from libtest import include
include("gen/tree")


def height(n, edges):
    par = to_parents(n, edges)
    depth = [0] * n
    for v in range(1, n):  # canonical labels: parent < child
        depth[v] = depth[par[v]] + 1
    return max(depth)


for kind, _ in edge_case_trees(7):
    h = height(7, rand_tree(7, kind, shuffle=False))
    print(kind, h if kind in ("path", "star", "binary", "broom", "spider", "double-star") else 1 <= h <= 6)
print(fmt_edges(rand_tree(4, "path", shuffle=False)))
