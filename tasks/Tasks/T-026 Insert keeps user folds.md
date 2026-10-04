---
status: done
created: 2026-10-04
updated: 2026-10-04
tags: [task, cpp, library]
---
# T-026 Insert keeps user folds

## Goal
Inserting a template from the library (`<leader>rl`) must collapse only the inserted template, never
the user's own functions (`solve()`, `main()`, helpers).

## Cause
Reproduced in a pty session driving the real picker: after inserting `Segment tree` above `solve()`,
both the template and `solve()` come up collapsed. Calling `util.lib.insert` + `util.fold.close`
directly (Normal mode) does not reproduce it.

The picker confirms from its prompt, in Insert mode, and Neovim does not update expr folds in Insert
mode. So when `fold.close` runs, the folds are stale: the fold of `solve()` was stretched over the
lines just inserted above it (one level-1 fold over 17..102). `foldclose` on the template's Title
line closes that stale fold, and when the folds are recomputed after leaving Insert mode the real
`solve()` fold keeps the closed state.

## Plan
- [x] Test: insert a template above a user fold and call `close` from Insert mode; the user fold
  must stay open and the template must be collapsed (once Insert mode is left).
- [x] `fold.close` in Insert mode waits until Insert mode is left (folds are up to date then).
- [x] Verify with the real picker in a pty session.

## Pipeline
- [x] Task read, requirements and plan written (status `doing`)
- [x] Branch `task/T-026-insert-keeps-user-folds`
- [x] Tests written and failing for the right reason
- [x] Implemented; `python3 tests/run.py` passes in full
- [x] Vault updated (log, guides, decisions)
- [x] Committed, merged into `main` (ff-only), branch deleted

## Log
- 2026-10-04: created; cause reproduced (see above).
- 2026-10-04: test in `tests/nvim/fold_test.lua` inserts a template above `solve()` and calls
  `close` from Insert mode (`i<Cmd>...<CR><Esc>`); it failed with `solve()` collapsed together with
  the stale fold (15..22). Found that a forced recompute (`foldexpr = foldexpr`) does nothing in
  Insert mode, and that after leaving it the folds stay stale until one. Implemented: `close` in
  Insert/Replace mode waits for `ModeChanged` out of it, recomputes the folds of the buffer's
  template windows (`opt_local`, not touching the global `foldexpr`), then collapses. Verified with
  the real picker in a pty (Segment tree, Min-cost flow, DSU with rollback above `solve()`): only
  the template collapses. Full suite passes. Decision [[D-021 Collapse templates outside Insert mode]].

## Related
- Tasks: [[T-025 Fold only templates]], [[T-017 Collapsible templates]]
- Decisions: [[D-021 Collapse templates outside Insert mode]], [[D-020 Only templates open collapsed]]
- Guides: [[Template Library]]
