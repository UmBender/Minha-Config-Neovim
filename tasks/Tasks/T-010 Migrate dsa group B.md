---
status: done
created: 2026-10-01
updated: 2026-10-01
tags: [task, cpp, notebook, agent]
---
# T-010 Migrate dsa group B

## Goal
Apply [[D-010 Template presets, examples and generated docs]] to: `binary-search, ternary-search, lis, fast-subset-sum, cartesian-tree, li-chao, simplex, wavelet-matrix` (all in `lib/dsa`).
Runs in a background agent, in its own git worktree.

## Plan
- [x] binary-search: no preset (the predicate is the input); example
- [x] ternary-search: no preset (the function is the input); example
- [x] lis: `lisLength(a, strict, cmp)`; example
- [x] fast-subset-sum: `ss.maxAtMost(s)` (closest sum <= s, e.g. balanced partition); example
- [x] cartesian-tree: `minCartesianTree(a)` / `maxCartesianTree(a)` (parents); example
- [x] li-chao: default `T = long long`, default range `[-1e9, 1e9]`, `MinLiChao` / `MaxLiChao` aliases; example
- [x] simplex: no preset (plain values already); example
- [x] wavelet-matrix: `DistinctCount dc(a); dc.query(l, r)` (range distinct count); example
- [x] Remove `// Pending:` from all 8, `--write-docs`, full suite

## Pipeline
- [x] Task read, requirements and plan written (status `doing`)
- [x] Branch `task/T-010-...`
- [x] Tests written and failing for the right reason
- [x] Implemented; `python3 tests/run.py` passes in full
- [x] Vault updated (log)
- [x] Committed on the branch (merged into `main` by the main session)

## Log
- 2026-10-01: created by the main session.
- 2026-10-01: presets + tests written first (lisLength, maxAtMost, min/maxCartesianTree, LiChao defaults
  + MinLiChao/MaxLiChao, WaveletMatrix CTAD + DistinctCount), examples for all 8, `// Pending:` removed.
  Session interrupted during mutation checks; resumed by a second agent, which found three mutations
  still applied (maxAtMost `s > 1`, maxCartesianTree `<=`, DistinctCount `l + 1`). The tests caught
  all three, and they were reverted.
- 2026-10-01: li-chao: the default range was `[-1e9, 1e9)`, so x = 1e9 was outside despite the docs
  saying `|x| <= 1e9` (the old test passed only because whole-range lines sit at the root). The default
  `hi` is now `1e9 + 1`, with a test covering segments at x = 1e9 (it failed before the fix).
- 2026-10-01: mutation checks: li-chao default hi, MaxLiChao alias, lisLength empty/max were all caught.
  `python3 tests/run.py dsa` passes in full; `--write-docs` regenerated the 8 pages (catalog left to the merge).
