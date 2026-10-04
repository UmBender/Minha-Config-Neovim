---
status: done
created: 2026-10-04
updated: 2026-10-04
tags: [task, cpp, library]
---
# T-025 Fold only templates

## Goal
Only inserted library templates should open collapsed. The user's own code (`solve()`, `main()`,
helpers) must never come up collapsed: not when the file is reopened, not while typing.

## Cause
Reproduced in a pty session: opening a file with a template collapses `solve()` and `main()` too.
Neovim gives a new fold the state of the sibling fold above it (`fold.c`: "The new fold is closed
if the fold above it is closed"). Templates are collapsed in the ftplugin, before Treesitter
parses ([[D-018 Paint first, Treesitter after]]), so the Treesitter folds of the user's code are
created afterwards as siblings of a closed template and inherit "closed", one after another. The
same happens while typing whenever a fold disappears and comes back (unbalanced braces, ...).

## Plan
- [x] Each template becomes two folds on the same lines: an outer level-1 wrapper that stays open
  and the inner level-2 template fold that is collapsed. User folds are siblings of the open
  wrapper, so they inherit "open". Inner Treesitter levels shift by two.
- [x] `close` keeps the wrapper open (e.g. after `zc` twice) and collapses the inner fold.
- [x] Tests: user folds created after the templates are collapsed (late Treesitter parse, typing
  below a template) stay open; level layout updated.
- [x] D-020 (supersedes the fold layout part of D-014), guide note.

## Pipeline
- [x] Task read, requirements and plan written (status `doing`)
- [x] Branch `task/T-025-fold-only-templates`
- [x] Tests written and failing for the right reason
- [x] Implemented; `python3 tests/run.py` passes in full
- [x] Vault updated (log, guides, decisions)
- [x] Committed, merged into `main` (ff-only), branch deleted

## Log
- 2026-10-04: created; cause reproduced (lines of `solve()`/`main()` report `foldclosed` != -1
  right after opening a file with one template).
- 2026-10-04: tests first in `tests/nvim/fold_test.lua`: late Treesitter parse, typing a function
  below a collapsed template, `close` after the wrapper was closed; they failed with the user's
  folds closed. Implemented the wrapper + inner fold layout (`>2` on Title, levels shifted by two)
  and `close` reopening the wrapper. Level-layout and `shift` tests changed deliberately (one more
  level inside templates). Verified in a pty session: the template opens collapsed, `solve()` and
  `main()` open; typing `void f() {...}` below the template stays open; `<leader>rf` still toggles
  only templates. Full suite passes. Decision [[D-020 Only templates open collapsed]].

## Related
- Tasks: [[T-017 Collapsible templates]]
- Decisions: [[D-020 Only templates open collapsed]], [[D-014 Collapsible templates with native folds]]
- Guides: [[Template Library]]
