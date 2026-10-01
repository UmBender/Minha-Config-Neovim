---
status: done
created: 2026-10-01
updated: 2026-10-01
tags: [task, cpp, library]
---
# T-015 Template variant menu

## Goal
Instead of packing every common use of a data structure into its file as presets
([[D-010 Template presets, examples and generated docs]]), the picker (`<leader>rl`) offers a
**menu of variants** for a structure, and the user picks one:

- **normal**: the generic template (as today, without the bundled presets)
- **educational**: the same structure, commented to explain how and why it works
- **common**: a ready-made version for a common use (e.g. range add + range sum segment tree)

## Decisions from the user (2026-10-01)
- **Common: several per structure** (e.g. segtree -> sum / min / max; lazy segtree -> add+sum,
  assign+min). The second menu lists them next to normal and educational.
- **Common variants are specialized standalone code**: a short plain copy for that one use (e.g. a
  sum-only segtree without lambdas), not the generic template + a preset.
- **Existing `// Presets:` move out** of the main files into common variants; normal = only the
  generic structure.
- **Scope: only where it helps**: data structures with clear common uses (segtrees, Fenwick, DSU,
  sparse table, ...). Templates without variants skip the menu and insert directly.

## Choices made while implementing (see [[D-013 Template variants]])
- Layout: `<name>.cpp` normal, `<name>.edu.cpp` educational, `<name>.<use>.cpp` common; the
  variant name comes from the file name (no `// Variant:` key).
- Educational: same Title as normal (same names, so insertion dedups them); tested by compiling
  the normal test with the include redirected to `.edu.cpp`. Common: own Title, test, example.
- Docs: one page per structure with a *Variants* section; the catalog row lists the variants.
- The `// Presets` header key is rejected by lint from now on.

## Variants to write
Normal files lose their presets. Educational (`edu`) for the core structures; common uses where a
structure had presets:
- [x] `dsa/segtree`: edu, sum, min, max
- [x] `dsa/lazy-segtree`: edu, add-sum, add-min, add-max, assign-sum, assign-min, assign-max
- [x] `dsa/fenwick-tree`: edu, range (range add + range sum)
- [x] `dsa/dsu`: edu
- [x] `dsa/sparse-table`: edu, min, max, gcd
- [x] `dsa/max-add-segtree`: min
- [x] `dsa/suffix-max`: suffix-min, prefix-max, prefix-min
- [x] `dsa/wavelet-matrix`: distinct
- [x] `dsa/offline-deletions`: connectivity
- [x] `dsa/lis`: length
- [x] `graph/bfs01`: bfs
- [x] `graph/hungarian`: max
- [x] `dsa/fast-subset-sum`, `dsa/rbst`, `dsa/treap`: presets were only methods / default
  arguments, folded into `Usage:` (no variants)
- [x] Found while migrating (had presets too): `dsa/zeta`: gcd-conv, lcm-conv; `dsa/cartesian-tree`:
  min, max; `dsa/li-chao`: `MinLiChao`/`MaxLiChao` aliases dropped, defaults documented in `Usage:`

## Plan
- [x] Decisions from the user (above)
- [x] Write them as a D-NNN that supersedes the presets part of D-010 ([[D-013 Template variants]])
- [x] nvim tests + fixtures for variants (list, menu order, insert)
- [x] Runner: lint/standalone/tests/examples/docs aware of variants
- [x] `lua/util/lib.lua`: list variants, two-step picker (structure, then variant), insertion
- [x] Migrate templates (list above), tests first per variant
- [x] Guides: [[Template Library]], [[Keymaps]]

## Pipeline
- [x] Task read, requirements and plan written (status `doing`)
- [x] Branch `task/T-015-variant-menu`
- [x] Tests written and failing for the right reason
- [x] Implemented; `python3 tests/run.py` passes in full
- [x] Vault updated (log, guides, decisions)
- [x] Committed, merged into `main` (ff-only), branch deleted

## Log
- 2026-10-01: created. Order requested by the user: [[T-014 Performance]], then this, then
  [[T-016 Port strings templates]].
- 2026-10-01: user answered the open decisions (see above).
- 2026-10-01: started. Open choices settled in [[D-013 Template variants]].
- 2026-10-01: `lua/util/lib.lua`: `list()` returns structures with `variants` (edu first, then by
  name) and a `search` text covering them; `menu(item)` builds the second menu; `pick()` opens it
  for structures with variants. nvim tests + fixtures (`x/base.edu`, `x/base.one`, `x/base.two`).
  Checked in a real pty session: segtree -> sum inserts only `SumSegtree`.
- 2026-10-01: `tests/run.py`: variant ids (`<base>.<variant>`), lint (edu keeps the base Title, has
  no own test/example; `// Presets` rejected; variant needs its base), edu tests via an include
  overlay (`tests/.build/overlay/`), docs: one page per structure with a *Variants* table and a
  section per common variant, catalog gets a *Variants* column.
- 2026-10-01: migrated all 17 templates that had presets (list above) and wrote 5 educational
  variants. Tests changed deliberately: preset checks moved out of the normal tests into the
  variant tests (default-argument checks stayed). Mutation-checked several variants (and the edu
  overlay). The segtree test missed a non-commutative bug on the right side of `query` (found by
  mutating the edu version): added a random affine-composition stress to `tests/dsa/segtree.cpp`.
- 2026-10-01: full suite passes (74 templates incl. variants). Guides updated.

## Related
- Decisions: [[D-013 Template variants]], [[D-009 Template library format]],
  [[D-010 Template presets, examples and generated docs]]
- Guides: [[Template Library]]
