---
status: done
created: 2026-10-01
updated: 2026-10-01
tags: [task, cpp, notebook]
---
# T-018 Port math templates

## Goal
Port `notebook/content/math` (15 templates: berlekamp-massey, crt, egcd, factor, floor-sum,
gaussian-elimination, lagrangian-interpolation, lagrangian-polynomial, linear-recurrence, mod-sqrt,
perm-int, powm, primality, sieve, xor-basis) to `lib/math/` with tests, examples and docs, in the
format in force ([[D-009 Template library format]], [[D-013 Template variants]]).

## Requirements
- One template per notebook file, header + test (edge cases + stress vs brute force) + example.
- **No global `mod`**: the notebook hardcodes `const ll mod` in `powm` and every modular template
  uses it. Here the modulus is a trailing argument `long long mod = 998244353` (runtime, so a
  modulus read from the input works, e.g. `sqrtMod(a, p)` per query). Moduli `< 2^31` (products fit
  in `long long`), except the 64-bit number theory (`primality`, `factor`, `crt`), which uses
  `__int128` products.
- Inverses modulo a prime via `powMod(x, mod - 2, mod)`; modular templates `Requires: math/powm`
  instead of copying it.
- Variants: none planned. These are single-purpose functions without the "same structure, different
  operation" shape of [[D-013 Template variants]].

## Plan

| Notebook | Library | Changes |
| -------- | ------- | ------- |
| powm | `math/powm` | `powMod(b, e, mod)` (reduces `b`, negative `b` OK) |
| egcd | `math/egcd` | `egcd(a, b, x, y)`, plus `invMod(a, m)` for any modulus (`-1` if not coprime) |
| crt | `math/crt` | `crt(r1, m1, r2, m2)` -> `{r, lcm}` or `{0, -1}`; `crt(rs, ms)` for a list; fix `q * x` overflow for moduli > 2^31 (`__int128`) |
| primality | `math/primality` | `isPrime(n)` deterministic for all `uint64` (`n < 2` handled), `mulMod64`, `powMod64` |
| factor | `math/factor` | `factor(n)` -> sorted prime factors with multiplicity (Pollard rho, batched gcd, deterministic), `Requires: math/primality` |
| sieve | `math/sieve` | `Sieve(n)`: `primes`, `spf`, `isPrime(x)`, `factor(x)` (sorted, via `spf`) |
| floor-sum | `math/floor-sum` | one recursion returning `{sum f, sum i*f, sum f^2}` (the notebook's three mutually recursive functions call each other with Fibonacci-like growth); any sign of `a`, `b` |
| mod-sqrt | `math/mod-sqrt` | `sqrtMod(a, p)` (Tonelli-Shanks), smallest root or `-1`, `Requires: math/powm` |
| gaussian-elimination | `math/gaussian-elimination` | `gaussMod(a, mod)` RREF + rank; `detMod(a, mod)`; `solveMod(A, b, mod)` -> `optional` solution |
| lagrangian-interpolation | `math/lagrangian-interpolation` | `lagrange(xs, ys, x, mod)` O(n^2) (one inverse per point), `lagrange(ys, x, mod)` O(n) for `xs = 0..n-1` |
| lagrangian-polynomial | `math/lagrangian-polynomial` | `lagrangePoly(xs, ys, mod)` -> coefficients, O(n^2) |
| berlekamp-massey | `math/berlekamp-massey` | `berlekampMassey(s, mod)` -> `c` with `s[i] = sum c[j] s[i-1-j]` |
| linear-recurrence | `math/linear-recurrence` | `linRec(s, c, k, mod)`: k-th term (0-indexed), O(n^2 log k); empty `c` handled |
| perm-int | `math/perm-int` | `perm2int(p)` / `int2perm(n, k)`: lexicographic rank of a permutation of `0..n-1` |
| xor-basis | `math/xor-basis` | `XorBasis<B = 64>` over `uint64`: `add`, `contains`, `minXor`, `maxXor`, `size`, `kth` (k-th smallest of the span) |

## Pipeline
- [x] Task read, requirements and plan written (status `doing`)
- [x] Branch `task/T-018-port-math`
- [x] Tests written and failing for the right reason
- [x] Implemented; `python3 tests/run.py` passes in full
- [x] Mutation check
- [x] Vault updated (log, guides, decisions), docs regenerated
- [x] Committed, merged into `main` (ff-only), branch deleted

## Log
- 2026-10-01: created and started (next area of [[T-005 Notebook-based templates]] after
  [[T-016 Port strings templates]]). Read the notebook files and `main.tex`, wrote the plan above.
- 2026-10-01: tests written (`tests/math/*.cpp`: brute force / enumeration references, e.g. row
  spaces mod 2 and 3 by enumeration, shortest recurrence by enumeration, permutation-expansion
  determinants, trial division) and failing (no templates yet).
- 2026-10-01: implemented the 15 templates and examples; full suite passes (100 templates).
  Notes / differences from the notebook:
  - Modulus as a trailing argument, recorded in [[D-015 Modulus as an argument]].
  - `crt`: the notebook computed `q * x % k` in `long long`, which overflows once `m2 / g > 2^32`
    (e.g. moduli 1000 and 1e15); now `__int128`.
  - `floor-sum`: the notebook's `floorSumSq` / `floorSumI` call each other and `floorSum` on the
    same arguments, so the call tree grows like Fibonacci over the Euclid depth (O(m) calls on
    Fibonacci inputs). Now `floorSums` returns the three sums from one recursion; `floorSum` keeps
    its own cheaper recursion (it must not compute the square sum, which may overflow `__int128`
    when only the plain sum is wanted). Both accept negative `a`, `b` (floor division first).
  - `primality`: the notebook's `isPrime(0)` was wrong (no `n < 2` check); now 7 fixed bases
    (deterministic for 64 bits) after trial division by primes up to 37, taking `unsigned long long`.
  - `factor`: returns a sorted vector instead of appending to an output parameter; Pollard rho is
    deterministic (`c = 1, 2, ...` instead of a global RNG) with the gcd batched every 64 steps.
  - `linear-recurrence`: an empty recurrence crashed (`e[1]` out of bounds); now returns 0.
  - `berlekamp-massey`: kactl-style fixed-size buffers; the result is cut to the recurrence length
    (the notebook could return trailing zero coefficients).
  - `gaussian-elimination`: added `detMod` and `solveMod`; an empty matrix no longer reads `a[0]`.
  - `lagrangian-interpolation`: one modular inverse per point instead of one per pair.
  - `xor-basis`: generic bit width `XorBasis<B>` over `uint64`, plus `contains`, `size`, `kth`.
  - `sieve`: added `isPrime(x)`, `factor(x)`; `i * p` no longer overflows `int` near 2^31.
  - No variants (see Requirements).
- 2026-10-01: mutation check, 43 planted bugs. Two survived at first: `powMod(x, 0, 1)` returning 1
  (added the case) and dropping the `__int128` in `crt` (the stress never had `m2 / g > 2^32`:
  added a small-times-huge moduli stress). All caught now.

## Related
- Tasks: [[T-005 Notebook-based templates]], [[T-016 Port strings templates]]
- Decisions: [[D-009 Template library format]], [[D-013 Template variants]], [[D-015 Modulus as an argument]]
