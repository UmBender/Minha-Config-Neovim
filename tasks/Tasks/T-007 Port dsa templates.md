---
status: done
created: 2026-10-01
updated: 2026-10-01
tags: [task, cpp, notebook]
---
# T-007 Port dsa templates

## Goal
Port `notebook/content/dsa` (27 templates) to `lib/dsa/` following [[D-009 Template library format]]:
flexible, self-contained, and every template stress-tested against a brute force. Pilot area: the
user reviews the style before the other areas are ported.

## Plan

| Notebook | Library | Changes |
| -------- | ------- | ------- |
| binary-search | `dsa/binary-search` | `firstTrue`/`lastTrue` on any integer type (overflow-safe `midpoint`), `firstTrueReal` |
| ternary-search | `dsa/ternary-search` | integer `ternaryMax/Min` (plateaus after the optimum allowed), real versions |
| fenwick-tree | `dsa/fenwick-tree` | `Fenwick<T>`, O(n) build, `get`/`set`, `lowerBound` |
| segtree + simple-segtree | `dsa/segtree` | merged; lambdas, build from vector, `get`, `all`, `maxRight`/`minLeft` |
| lazy-segtree | `dsa/lazy-segtree` | lambdas, build from vector, point apply, `maxRight`/`minLeft` |
| max-add-segtree | `dsa/max-add-segtree` | `MaxAddSegtree<T>`, build from vector, `get` |
| sparse-table | `dsa/sparse-table` | lambda op, CTAD |
| dsu | `dsa/dsu` | `size`, `same`, component count, `groups` |
| (new) | `dsa/dsu-rollback` | DSU with `undo`/snapshots, used with offline deletions |
| mo-queries | `dsa/mo-queries` | `moOrder` + `mo(...)` runner with add/remove callbacks |
| lis | `dsa/lis` | generic `T`, strict/non-strict, comparator, `lisEnding` |
| fast-subset-sum | `dsa/fast-subset-sum` | `SubsetSum` struct: `can(s)`, `recover(s)` |
| cartesian-tree | `dsa/cartesian-tree` | comparator "i above j", root parent = -1 |
| fft | `dsa/fft` | `fftConv` (real) and `fftConvInt` (rounded integers) |
| ntt | `dsa/ntt` | self-contained, no 200 MB static arrays, `nttConv<MOD>` + `convMod` (any modulus, 3 primes + CRT) |
| xor-convolution | `dsa/xor-convolution` | xor/and/or convolutions, exact or modular |
| zeta | `dsa/zeta` | generic `T`, custom op for zeta, sub/superset + divisor/multiple transforms |
| subset-convolution | `dsa/subset-convolution` | self-contained, exact or modular |
| suffix-max | `dsa/suffix-max` | comparator for the value (max/min) |
| offline-deletions | `dsa/offline-deletions` | callback-based (`insert`, `undo`, `answer`), multiset semantics |
| rbst | `dsa/rbst` | `PersistentRBST<T>`: build, insert, erase, get, `toVector`, own RNG |
| treap | `dsa/treap` | `ImplicitTreap<T>`: insert/erase/get/set, reverse, range add, sum/min, no raw `new` |
| simplex | `dsa/simplex` | self-contained, typed results |
| wavelet-matrix + extended | `dsa/wavelet-matrix` | merged; any value type (compression), `kth`, counts, `prev`/`next`, `sum`, `visit` + `pos` |
| li-chao + extended | `dsa/li-chao` | merged; generic `T`, min or max, sparse range (any coordinates), segments |

## Pipeline
- [x] Task read, requirements and plan written (status `doing`)
- [x] Branch `task/T-007-port-dsa`
- [x] Tests written and failing for the right reason
- [x] Implemented; `python3 tests/run.py` passes in full
- [x] Vault updated (log, guides, decisions)
- [x] Committed, merged into `main` (ff-only), branch deleted

## Log
- 2026-10-01: created. Bugs found while reading the notebook (tests must cover them):
  `binSearch` computes `mid` as `int` from `ll` bounds (overflow); `ternarySearch` returns `int`
  for `ll` ranges; `divZeta`/`mulMobius` loop `i * p < n`, which looks like it skips index `n`; `ntt` allocates
  three static `1 << 23` arrays (~200 MB) and depends on an external `powm`; `rbst::node` takes `ll`
  instead of `T`.
- 2026-10-01: tests written first for all 25 (each failed on the missing template). Merged:
  `simple-segtree` into `segtree` (generic), `li-chao-extended` into `li-chao` (`addSegment`),
  `wavelet-matrix-extended` into `wavelet-matrix` (`visit` + `pos`). New: `dsu-rollback`
  (needed by `offline-deletions`).
- Issues found while implementing:
  - Harness: with `-I tests`, a test including a missing template included **itself** (infinite
    recursion in the compiler). Moved `test.h` to `tests/include/`.
  - Harness: `CHECK` was a statement macro, so it broke in comma expressions. All checks are now expressions.
  - GCC 16: `bits/stdc++.h` no longer includes `<cassert>`, so `assert` is undefined. Templates
    don't use it; lint rejects it.
  - UBSan: `ImplicitTreap<int>` sums overflowed. Sums of integer types are now `long long`.
  - UBSan: the wavelet matrix overflowed when building prefix sums of values near 1e18 (even when
    `sum` is never used). Sums now wrap in `unsigned long long`, so `sum` is exact whenever the answer fits.
- Mutation check: reintroduced 8 bugs (the notebook's int `mid`, the `i * p < n` bound in zeta,
  `<=` in ternary search, a missing push in the lazy `maxRight`, an off-by-one in wavelet `visit`,
  a flipped Li Chao side, a missing reverse push in the treap, a wrong offline-deletion span end).
  All 8 were caught; the int `mid` loops forever and shows up as a timeout (per-test timeout lowered to 60 s).
- Full suite: 25 templates + nvim tests pass (about 35 s with cached builds, about 2.5 min cold).

## Related
- Decisions: [[D-009 Template library format]], [[D-008 Task pipeline]]
- Guides: [[Template Library]]
