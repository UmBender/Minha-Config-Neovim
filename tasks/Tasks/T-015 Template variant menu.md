---
status: todo
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

## Still open (decide while implementing, log the choice)
- File layout for variants (proposal: `lib/<area>/<name>.cpp` normal, `<name>.edu.cpp`,
  `<name>.<use>.cpp`, with a `// Variant:` header key).
- Testing: educational variants should pass the generic test unchanged (compile the same test
  against them); each common variant needs its own small test + stress vs brute force.
- Examples/docs: one example per variant, or per structure with a section per variant.

## Plan
- [x] Decisions from the user (above)
- [ ] Write them as a D-NNN that supersedes the presets part of D-010
- [ ] Runner: lint/standalone/tests/examples/docs aware of variants
- [ ] `lua/util/lib.lua`: list variants, two-step picker (structure, then variant), insertion
- [ ] Migrate templates
- [ ] Guides: [[Template Library]], [[Keymaps]]

## Pipeline
- [ ] Task read, requirements and plan written (status `doing`)
- [ ] Branch `task/T-015-<slug>`
- [ ] Tests written and failing for the right reason
- [ ] Implemented; `python3 tests/run.py` passes in full
- [ ] Vault updated (log, guides, decisions)
- [ ] Committed, merged into `main` (ff-only), branch deleted

## Log
- 2026-10-01: created. Order requested by the user: [[T-014 Performance]], then this, then
  [[T-016 Port strings templates]].
- 2026-10-01: user answered the open decisions (see above).

## Related
- Decisions: [[D-009 Template library format]], [[D-010 Template presets, examples and generated docs]]
- Guides: [[Template Library]]
