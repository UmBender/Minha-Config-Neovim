---
status: done
created: 2026-10-01
updated: 2026-10-01
tags: [task, cpp, notebook, agent]
---
# T-009 Migrate dsa group A

## Goal
Apply [[D-010 Template presets, examples and generated docs]] to: `fenwick-tree, max-add-segtree, sparse-table, dsu, dsu-rollback, offline-deletions, mo-queries, suffix-max` (all in `lib/dsa`).
Runs in a background agent, in its own git worktree.

## Plan
- [x] For each template: presets for its most common uses (tests first), example, remove `// Pending:`
  - [x] fenwick-tree: default `T = long long`; `RangeFenwick` (range add + range sum, two BITs)
  - [x] max-add-segtree: default `T = long long`; `MinAddSegtree` (min variant)
  - [x] sparse-table: `minSparseTable(a)`, `maxSparseTable(a)`, `gcdSparseTable(a)`
  - [x] dsu: no presets (not generic); example only
  - [x] dsu-rollback: no presets (not generic); example only
  - [x] offline-deletions: dynamic connectivity helpers `componentCounts(od, dsu)` / `connectedAt(od, dsu, ask)`
  - [x] mo-queries: no presets (callbacks are the problem); example only
  - [x] suffix-max: defaults `K = V = long long` and identity by comparator; `SuffixMin`, `PrefixMax`, `PrefixMin`
- [x] `--write-docs`, full suite

## Pipeline
- [x] Task read, requirements and plan written (status `doing`)
- [x] Branch `task/T-009-migrate-dsa-a`
- [x] Tests written and failing for the right reason
- [x] Implemented; `python3 tests/run.py` passes in full
- [x] Vault updated (log)
- [x] Committed on the branch (merged into `main` by the main session)

## Log
- 2026-10-01: created by the main session.
- 2026-10-01: (agent, first run) wrote the preset tests and presets for all 8 templates, removed
  `// Pending:`; interrupted before the examples.
- 2026-10-01: (agent, resumed) fixed `PrefixMin`: it was an alias of `SuffixMin` (missing
  `greater<K>`); the new test already caught it. Wrote `examples/dsa/*.cpp` for the 8 templates
  (fenwick: `RangeFenwick`; max-add: max + `MinAddSegtree`; sparse table: min/max/gcd presets + OR
  general form; dsu: counts/sizes/groups; dsu-rollback: `time()`/`rollback()` for temporary edges;
  offline-deletions: `componentCounts`/`connectedAt`; mo: distinct values; suffix-max:
  `SuffixMax`/`PrefixMax`). Note: with CTAD, `SuffixMax sm(-1)` deduces `V = int`; the example
  passes `-1LL`. Regenerated the 8 `tasks/Library/dsa/` pages (catalog left for the merge).
  `python3 tests/run.py dsa` passes in full.
- 2026-10-01: mutation check (each caught): RangeFenwick `b2` update and `sum` sign, MinAddSegtree
  init/add negation, `gcdSparseTable` op, `connectedAt` pair, `componentCounts` without undo,
  `PrefixMin` key order, `worst()` for `greater`.
