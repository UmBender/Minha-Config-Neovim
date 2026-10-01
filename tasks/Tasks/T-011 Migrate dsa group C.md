---
status: done
created: 2026-10-01
updated: 2026-10-01
tags: [task, cpp, notebook, agent]
---
# T-011 Migrate dsa group C

## Goal
Apply [[D-010 Template presets, examples and generated docs]] to: `fft, ntt, xor-convolution, zeta, subset-convolution, rbst, treap` (all in `lib/dsa`).
Runs in a background agent, in its own git worktree.

## Requirements
- Presets only where useful (plain values in/out, same lib file, `// Presets:` header key), tested
  with stress tests against brute force.
- One runnable example per template (`Problem:`/`Input:`/`Output:`), presets first.
- Remove `// Pending:`; regenerate docs; full `python3 tests/run.py` green.
- Don't touch `Title`/`Description`, nor files outside the 7 templates.

## Plan
- [x] fft: no lambdas to hide (fftConv/fftConvInt are the common uses) -> example only
- [x] ntt: nttConv/convMod already the common uses -> example only
- [x] xor-convolution: xorConv/andConv/orConv already are the presets -> example only
- [x] zeta: presets `gcdConv(a, b[, mod])`, `lcmConv(a, b[, mod])` + example
- [x] subset-convolution: no lambdas -> example only
- [x] rbst: default `T = long long` (`PersistentRBST t;`) + example
- [x] treap: default `T = long long` (`ImplicitTreap t;`) + example
- [x] Remove `// Pending:` from all 7, `--write-docs`, full suite

## Pipeline
- [x] Task read, requirements and plan written (status `doing`)
- [x] Branch `task/T-011-...`
- [x] Tests written and failing for the right reason
- [x] Implemented; `python3 tests/run.py` passes in full
- [x] Vault updated (log)
- [x] Committed (merged into `main` by the main session)

## Log
- 2026-10-01: created by the main session.
- 2026-10-01: presets and tests (agent, resumed after an interruption):
  - zeta: `gcdConv` / `lcmConv` (shared helper `divisorConv`), stress-tested against an O(n^2) brute
    force, exact and mod 998244353; extra case with unreduced (negative / ~1e18) inputs under a mod.
    Also added the lcm_convolution Verify link.
  - rbst / treap: `T = long long` default, tested with `static_assert` + values above 2^31.
  - fft, ntt, xor-convolution, subset-convolution: the existing functions already are the plain
    one-liners, so no extra presets; example only.
- 2026-10-01: examples `examples/dsa/{fft,ntt,xor-convolution,zeta,subset-convolution,rbst,treap}.cpp`,
  `// Pending:` removed from all 7, own pages regenerated with `--write-docs` (catalog left for the merge).
- 2026-10-01: mutation check on the zeta presets: dropping the input normalization or the
  post-transform normalization fails the suite (wrong values / UBSan overflow); swapping zeta kinds fails.
  Not zeroing `a[0]` and skipping `% mod` on the pointwise product are equivalent mutants
  (a[0] is multiplied by b[0] = 0; products stay below 2^63 for small n), so they were not counted.
- 2026-10-01: `python3 tests/run.py dsa` passes in full.
