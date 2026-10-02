# Problem: Project Euler 39: the perimeter p <= 1000 with the most right triangles with integer sides.
# Output:
#   840
from libtest import include
include("nt/pythagorean")
from collections import Counter

ways = Counter(a + b + c for a, b, c in triples(1000, "perimeter"))
print(ways.most_common(1)[0][0])
