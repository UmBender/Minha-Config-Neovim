---
status: done
created: 2026-10-14
updated: 2026-10-14
tags: [task, cpp, library, math]
---
# T-029 Modint

## Goal
Add a modular integer type to the C++ library (`lib/math/modint`), with a static modulus
(compile-time constant) and a dynamic one (set at runtime), modular inverses, and precomputed
inverses / factorials. Inspired by ACL's `modint.hpp` (`~/comp/refs/atcoder`), not in the notebook.

## Requirements
- One type for both: `ModInt<998244353>` (static) and `ModInt<0>` (dynamic, `setMod(m)`); other
  non-positive ids give independent dynamic moduli (ACL's `dynamic_modint<id>`).
- Arithmetic `+ - * /`, unary `-`, `++/--`, `==`, `pow`, `inv` (extended Euclid: any modulus where
  the value is invertible), `val`, stream `<<`/`>>`, construction from any integer (any sign).
- Modulus `1 <= m < 2^31`.
- Common-use variant `math/modint.comb`: inverses of `1..n` in O(n), factorials, inverse
  factorials, binomials `C(n, k)` / arrangements `P(n, k)`.
- Tests: fixed cases + stress against `__int128` brute force, both modes, several moduli
  (including composite and 1). Examples for both files.

## Plan
- [x] `tests/math/modint.cpp`, `tests/math/modint.comb.cpp` (fail first)
- [x] `lib/math/modint.cpp`, `lib/math/modint.comb.cpp`
- [x] Examples, mutation check, docs regenerated
- [x] D-023 (modint type next to D-015's modulus-as-argument convention), guides

## Pipeline
- [x] Task read, requirements and plan written (status `doing`)
- [x] Branch `task/T-029-modint`
- [x] Tests written and failing for the right reason
- [x] Implemented; `python3 tests/run.py` passes in full
- [x] Vault updated (log, guides, decisions)
- [x] Committed, merged into `main` (ff-only), branch deleted

## Log
- 2026-10-14: created.
- 2026-10-14: tests first (`tests/math/modint.cpp`: fixed cases + `__int128` stress over static
  moduli 1, 2, 6, 10, 2^30, 998244353, 1e9+7, 2^31-2, 2^31-1 and random dynamic moduli, two
  independent dynamic ids; `tests/math/modint.comb.cpp`: Pascal's triangle brute force, prebuilt and
  grown tables, dynamic primes). Both failed on the missing templates.
- 2026-10-14: `ModInt<M>` (one class, static for `M > 0`, dynamic for `M <= 0`, see
  [[D-023 Modint type]]) and `Comb<Mint>`. One wrong constant in the test (20! mod 998244353)
  fixed against Python. Mutation check: 8 plausible bugs (subtraction wrap, unchecked inverse,
  negative reduction, negative pow, 32-bit product, `>` vs `>=`, inverse-table sign, `P` formula),
  each failed the suite. Examples, docs regenerated, guide updated. Full suite passes.

## Related
- Decisions: [[D-015 Modulus as an argument]]
- Guides: [[Template Library]]
