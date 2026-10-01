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

## Open decisions (ask the user before starting)
- File layout for variants (e.g. `lib/dsa/segtree.cpp` + `segtree.edu.cpp` + `segtree.<use>.cpp`,
  or a folder per structure).
- "Common": one per structure or several (sum / min / max / add-sum ...)? Is it a self-contained
  specialized copy, or the generic template + a thin preset on top (inserted together)?
- Do educational variants need their own tests (they should compile and behave identically;
  cheapest is to run the generic test against them too)?
- Do the existing `// Presets:` move out of the main files into "common" variants?
- Migrate every existing template now, or only the ones that have presets?

## Plan
- [ ] Decisions from the user, written as a D-NNN superseding/extending D-010
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

## Related
- Decisions: [[D-009 Template library format]], [[D-010 Template presets, examples and generated docs]]
- Guides: [[Template Library]]
