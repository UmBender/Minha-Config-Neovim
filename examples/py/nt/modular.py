# Problem: For each query (n, k) print C(n, k) mod 10^9 + 7; then solve x = 2 (mod 3), x = 3 (mod 5),
#   x = 2 (mod 7) and the inverse of 3 mod 10.
# Input:
#   3
#   5 2
#   100000 50000
#   3 5
# Output:
#   10
#   149033233
#   0
#   23 105
#   7
from libtest import include
include("nt/modular")

MOD = 10**9 + 7
B = Binom(10**5, MOD)
q = int(input())
for _ in range(q):
    n, k = map(int, input().split())
    print(B.C(n, k))
print(*crt([2, 3, 2], [3, 5, 7]))
print(inv_mod(3, 10))
