---
status: accepted
date: 2026-10-01
tags: [decision, cpp, notebook]
---
# D-015 Modulus as an argument

## Context
The notebook's math templates share one global `const ll mod = 998244353; // Change this` (in
`powm`) and every modular routine reads it. That breaks for problems with a modulus from the input
(`sqrt_mod` has a prime per query), with two moduli at once, and when the solution already has its
own `MOD` constant. The library already had `convMod(a, b, m)` and `nttConv<MOD, G>` in `dsa/ntt`.

## Decision
- Modular templates take the modulus as a **trailing runtime argument with a default**:
  `long long mod = 998244353`. Changing it is a call-site choice (`powMod(b, e, 1000000007)`),
  nothing to edit in the inserted code.
- Values are `long long`, moduli `< 2^31` so that products fit. 64-bit number theory
  (`primality`, `factor`, `crt`) uses `__int128` products instead (`mulMod64`, `powMod64`).
- Inputs of any sign are reduced first, so callers don't normalize.
- Inverses modulo a prime: `powMod(x, mod - 2, mod)` from `math/powm`, pulled in through
  `Requires:`; any modulus: `invMod(a, m)` in `math/egcd`.
- No `modint` type for now: it isn't in the notebook. If one is added later, templates may grow an
  overload, but this calling convention stays.

## Consequences
- One inserted copy of a template serves every modulus in a solution.
- A default argument costs nothing at the call site for the common 998244353 case; for 1e9+7 the
  modulus is written at each call (or wrapped in a small lambda).
- The parameter is called `mod`: a solution with a global `mod` gets a `-Wshadow` warning only,
  which is harmless.

## Related
- Tasks: [[T-018 Port math templates]]
- Decisions: [[D-009 Template library format]]
