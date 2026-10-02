from libtest import include, check, check_eq, rnd

include("misc/matrix")


def brute_mul(A, B):
    return [[sum(A[i][k] * B[k][j] for k in range(len(B))) for j in range(len(B[0]))] for i in range(len(A))]


for _ in range(200):
    n, m, p = rnd.randint(1, 4), rnd.randint(1, 4), rnd.randint(1, 4)
    A = [[rnd.randint(-9, 9) for _ in range(m)] for _ in range(n)]
    B = [[rnd.randint(-9, 9) for _ in range(p)] for _ in range(m)]
    check_eq(mat_mul(A, B), brute_mul(A, B))
    check_eq(mat_mul(A, B, 7), [[x % 7 for x in r] for r in brute_mul(A, B)])
    S = [[rnd.randint(-3, 3) for _ in range(n)] for _ in range(n)]
    e = rnd.randint(0, 8)
    want = identity(n)
    for _ in range(e):
        want = brute_mul(want, S)
    check_eq(mat_pow(S, e), want)
    check_eq(mat_pow(S, e, 1000), [[x % 1000 for x in r] for r in want])
check_eq(identity(2), [[1, 0], [0, 1]])
check_eq(mat_pow([[5]], 0, 1), [[0]])

F = [0, 1]
while len(F) < 300:
    F.append(F[-1] + F[-2])
for n in range(300):
    check_eq(fib(n), F[n])
    check_eq(fib(n, 10**9 + 7), F[n] % (10**9 + 7))
check_eq(fib(10**18, 1), 0)

# linear recurrences: a(n) = 2 a(n-1) + 3 a(n-2), a(0)=1, a(1)=1; tribonacci
for c, init in [([2, 3], [1, 1]), ([1, 1, 1], [0, 0, 1]), ([5], [2]), ([0, 0, 1], [1, 2, 3])]:
    seq = list(init)
    while len(seq) < 60:
        seq.append(sum(cj * seq[-1 - j] for j, cj in enumerate(c)))
    for n in range(60):
        check_eq(lin_rec(c, init, n), seq[n], (c, n))
        check_eq(lin_rec(c, init, n, 97), seq[n] % 97, (c, n))
check_eq(lin_rec([1, 1], [0, 1], 90), F[90])
