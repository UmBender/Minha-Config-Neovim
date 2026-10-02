# Problem: Project Euler 66: the D <= 1000 whose minimal Pell solution has the largest x; and the
#   numerator of the 10th convergent of e = [2; 1, 2, 1, 1, 4, 1, 1, 6, ...] (PE 65 asks for the 100th).
# Output:
#   661
#   1457
from libtest import include
include("nt/cont-frac")

print(max((pell(D)[0], D) for D in range(2, 1001) if pell(D))[1])
e = [2] + [x for k in range(1, 40) for x in (1, 2 * k, 1)]
*_, (p, q) = convergents(e[:10])
print(p)
