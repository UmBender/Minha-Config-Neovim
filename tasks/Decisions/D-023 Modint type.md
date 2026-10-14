---
status: accepted
date: 2026-10-14
tags: [decision, cpp, library, math]
---
# D-023 Modint type

## Context
[[D-015 Modulus as an argument]] left the library without a `modint` type (the notebook has none):
modular templates take `long long` values and a trailing `mod`. Solutions full of modular
arithmetic (DP counts, combinatorics) read better with a type, and problems may fix the modulus at
compile time (998244353, 1e9+7) or read it from the input. ACL's `modint.hpp` solves both with two
classes (`static_modint<m>`, `dynamic_modint<id>`).

## Decision
- One class template `ModInt<M>` in `math/modint`: `M > 0` is a static modulus (`mod()` is
  `constexpr`), `M <= 0` a dynamic one set with `ModInt<M>::setMod(m)` (998244353 until set);
  different non-positive ids are independent moduli. One implementation instead of ACL's two
  near-copies; `setMod` only exists for the dynamic kind (`requires`).
- Moduli `1 <= m < 2^31`, values stored as `unsigned`, products via `unsigned long long %` (no
  Barrett: simpler, and fast enough for contest limits). Construction from any integer type and sign.
- `inv()` uses the extended Euclid algorithm, so it works for any modulus whenever
  `gcd(val, mod) = 1`; otherwise it returns 0 (templates can't `assert`). `pow` takes negative
  exponents as powers of the inverse.
- Precomputed inverses / factorials / binomials are the common-use variant `math/modint.comb`
  (`Comb<Mint>`, grows on demand, prime modulus larger than the indices), which requires
  `math/modint`.
- D-015 stands: the existing `long long` templates keep the modulus argument; a `ModInt` passes in
  through `val()` and comes back by construction.

## Consequences
- `using Mint = ModInt<998244353>;` is the whole setup in a solution; `ModInt<0>` covers input
  moduli without a second type.
- A wrong (non-invertible) division gives 0 silently, as ACL would only catch it with `assert`.
- A `Comb` of a dynamic modulus is tied to the modulus it was built with: build it after `setMod`.

## Related
- Tasks: [[T-029 Modint]]
- Decisions: [[D-015 Modulus as an argument]], [[D-013 Template variants]]
