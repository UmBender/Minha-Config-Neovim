---
status: done
created: 2026-10-01
updated: 2026-10-01
tags: [task, cpp, library]
---
# T-017 Collapsible templates

## Goal
Templates inserted with `<leader>rl` make solution files long, and most of the time only their
behavior matters, not their code. Inserted templates should **collapse to one line** that just
shows the template is there, and open on demand.

## Decisions from the user (2026-10-01)
- **End marker**: insertion appends `// End: <Title>` after each template, so the fold covers
  exactly the template even after edits, reformatting or reopening the file (one extra comment
  line per template in the submission).
- **Engine: native folds**, no new plugin (nvim-ufo was the alternative: manual foldmethod, extra
  dependency and startup cost, see [[D-012 Performance budget]]).

## Plan
- [x] `lua/util/lib.lua`: `insert` writes `// End: <Title>` after each template.
- [x] `lua/util/fold.lua`: template ranges (`// Title: X` .. `// End: X`), a foldexpr that adds
  them as level-1 folds on top of LazyVim's treesitter foldexpr (treesitter levels inside a
  template shift by one), `close` / `toggle` helpers.
- [x] `after/ftplugin/cpp.lua`: use that foldexpr, collapse templates when a file opens,
  `<leader>rf` toggles all template folds. Freshly inserted templates collapse right away.
- [x] Tests: `tests/nvim/fold_test.lua` + `lib_test.lua` (end marker).
- [x] Guides (Template Library, Keymaps), decision D-014.

## Pipeline
- [x] Task read, requirements and plan written (status `doing`)
- [x] Branch `task/T-017-fold-templates`
- [x] Tests written and failing for the right reason
- [x] Implemented; `python3 tests/run.py` passes in full
- [x] Vault updated (log, guides, decisions)
- [x] Committed, merged into `main` (ff-only), branch deleted

## Log
- 2026-10-01: created. Checked: with `foldtext=""` (LazyVim) a closed fold shows its first line
  with highlights, but eol virtual text is not drawn and ephemeral inline virtual text isn't
  either; `vim.treesitter.foldtext` is gone in 0.12. A custom `foldtext` would apply to every
  fold and lose the highlighted look, so a closed template shows its own `// Title: X` line.
- 2026-10-01: tests first (`tests/nvim/fold_test.lua`, End line in `lib_test.lua`), failing on
  the missing module / End line. Implemented `util/fold.lua`, End line in `util.lib.insert`,
  ftplugin wiring + `<leader>rf`. Mutation check: dropping the level shift fails 6 checks.
  Verified in a real pty session: LazyVim keeps our foldexpr, a file opens with its templates
  collapsed, `solve()` stays open, an inserted Fenwick tree collapses at once. Full suite passes.

## Related
- Decisions: [[D-014 Collapsible templates with native folds]]
- Guides: [[Template Library]], [[Keymaps]]
