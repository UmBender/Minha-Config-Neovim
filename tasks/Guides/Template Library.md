---
updated: 2026-10-01
tags: [guide, cpp, notebook]
---
# Template Library

Tested C++ templates in `lib/<area>/<name>.cpp`, based on the ICPC notebook but more flexible
(see [[D-009 Template library format]]).

## Using a template
1. In a `.cpp` buffer press `<leader>rl` and fuzzy-search by name, area or description.
   The preview shows the whole file, including the `Usage:` block.
2. `<CR>` inserts it **above `void solve()`** (or `int main()`, or below the cursor), together with
   any templates it `Requires`. Templates already in the file are skipped.
3. Read the `Usage:` comment at the top of the inserted code.

Conventions in every template: 0-indexed, half-open ranges `[l, r)`, generic types, operations
passed as lambdas.

## Running the tests

```sh
python3 tests/run.py            # everything: lint, standalone compile, C++ tests, nvim tests
python3 tests/run.py segtree    # only ids containing "segtree"
python3 tests/run.py nvim       # only the Neovim helper tests
```

All tests run under `-Werror`, ASan/UBSan and `_GLIBCXX_DEBUG`. Builds are cached, so only changed
tests recompile.

## Adding or changing a template
Follow [[D-008 Task pipeline]]: test first, then code, then the full suite.
1. `tests/<area>/<name>.cpp`: edge cases + random stress against a brute force.
2. `lib/<area>/<name>.cpp`: header (`Title`, `Description`, `Usage`, `Complexity`, optional
   `Verify`/`Requires`) + code. No `#include`/`#define`.
3. `python3 tests/run.py` must pass in full.

## Catalog

| Area | Status | Task |
| ---- | ------ | ---- |
| dsa | done (25 templates) | [[T-007 Port dsa templates]] |
| graph | todo | |
| math | todo | |
| strings | todo | |
| geometry | todo | |
| tree | todo | |

### dsa

| Template | What it gives you |
| -------- | ----------------- |
| `binary-search` | `firstTrue`/`lastTrue` on any integer type (overflow-safe), `firstTrueReal` |
| `ternary-search` | `ternaryMax`/`ternaryMin` on integers (plateaus after the optimum OK), real versions |
| `fenwick-tree` | `Fenwick<T>`: add/set/get, prefix and range sums, `lowerBound`, O(n) build |
| `segtree` | `Segtree(a, e, op)`: set/get/query/all, `maxRight`/`minLeft` |
| `lazy-segtree` | `LazySegtree(a, e, id, op, mapping, compose)`: range apply/query, binary search |
| `max-add-segtree` | ready-made range add + range max |
| `sparse-table` | `SparseTable(a, op)`: O(1) idempotent range queries |
| `dsu` | `DSU`: unite/same/find/size/count/groups |
| `dsu-rollback` | `RollbackDSU`: undo, `time()` snapshots, `rollback(t)` |
| `mo-queries` | `mo(qs, n, add, remove, answer)` (or 4 side-specific callbacks), `moOrder` |
| `lis` | `lis(a, strict, cmp)` indices, `lisEnding` lengths |
| `fast-subset-sum` | `SubsetSum(w)`: `can(s)`, `recover(s)` in O(nS/64) |
| `cartesian-tree` | parents from a comparator "i above j" |
| `fft` | `fftConv` (reals), `fftConvInt` (rounded integers) |
| `ntt` | `nttConv<MOD, G>`, `convMod(a, b, any m)` |
| `xor-convolution` | `xorConv`/`andConv`/`orConv`, exact or modulo |
| `zeta` | subset/superset zeta (custom op) + Mobius, divisor/multiple zeta + Mobius |
| `subset-convolution` | `subsetConvolution(a, b[, mod])` |
| `suffix-max` | best y over keys x >= X, online (max or min) |
| `offline-deletions` | `OfflineDeletion<V>`: insert/remove/query, then `run(ins, undo, answer)` |
| `rbst` | `PersistentRBST<T>`: persistent sequence, split/merge, huge shared sizes |
| `treap` | `ImplicitTreap<T>`: insert/erase, reverse, range add, sum/min |
| `simplex` | `Simplex(A, b, c).solve(x)`: LP max c.x, A x <= b, x >= 0 |
| `wavelet-matrix` | `kth`, `countLess`, `count`, `prev`/`next`, `sum`, `visit` + `pos` |
| `li-chao` | `LiChao<T, MAX>(lo, hi)`: lines and segments, any coordinate range |

> [!warning] `assert` with GCC 16
> `bits/stdc++.h` no longer includes `<cassert>`. If a solution uses `assert`, add `#include <cassert>`.
