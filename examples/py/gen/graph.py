# Problem: Generate a connected weighted graph and a DAG for a stress test, and check them the way a
#   brute force would before trusting them.
# Output:
#   6 9
#   connected True
#   dag True 6 10
from libtest import include
include("gen/graph")
import random

random.seed(3)
n, m = 6, 9
g = weighted(rand_graph(n, m, connected=True), 1, 100)
print(n, len(g))
print("connected", is_connected(n, g))
d = rand_dag(n, 10)
print("dag", is_dag(n, d), n, len(d))
# in a real generator: print(n, m); print(fmt_edges(g))
